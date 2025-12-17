#include "mainwindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initPartDatabase();
    setupUI();

    chart = new QChart();
    series = new QBarSeries();
    chart->addSeries(series);
    chart->setTitle("Part Condition Monitor");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    axisX = new QBarCategoryAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Wear Percentage (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initPartDatabase()
{
    partDatabase.insert("Engine Oil (Synthetic)", 10000);
    partDatabase.insert("Engine Oil (Conventional)", 5000);
    partDatabase.insert("Tires (All-Season)", 60000);
    partDatabase.insert("Brake Pads (Front)", 50000);
    partDatabase.insert("Brake Pads (Rear)", 70000);
    partDatabase.insert("Brake Fluid", 40000);
    partDatabase.insert("Engine Air Filter", 30000);
    partDatabase.insert("Cabin Air Filter", 25000);
    partDatabase.insert("Timing Belt", 100000);
    partDatabase.insert("Spark Plugs (Long Life)", 100000);
    partDatabase.insert("Transmission Fluid", 90000);
    partDatabase.insert("Coolant (Flush)", 80000);
    partDatabase.insert("Fuel Filter", 60000);
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    QFormLayout *formLayout = new QFormLayout();

    partSelector = new QComboBox();
    partSelector->addItems(partDatabase.keys());

    lastReplacedInput = new QLineEdit();
    currentKmInput = new QLineEdit();

    lastReplacedInput->setValidator(new QIntValidator(0, 9999999, this));
    currentKmInput->setValidator(new QIntValidator(0, 9999999, this));

    formLayout->addRow("Select Part:", partSelector);
    formLayout->addRow("Last Service (km):", lastReplacedInput);
    formLayout->addRow("Current Odometer (km):", currentKmInput);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnCalculate = new QPushButton("Calculate Wear");
    btnPseudocode = new QPushButton("View Pseudocode");
    btnLayout->addWidget(btnCalculate);
    btnLayout->addWidget(btnPseudocode);
    mainLayout->addLayout(btnLayout);

    partsTable = new QTableWidget();
    partsTable->setColumnCount(5);
    QStringList headers;
    headers << "Part" << "Lifespan (km)" << "Status" << "Wear %" << "Action";
    partsTable->setHorizontalHeaderLabels(headers);
    partsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(partsTable);

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300);
    mainLayout->addWidget(chartView);

    connect(btnCalculate, &QPushButton::clicked, this, &MainWindow::calculateWear);
    connect(btnPseudocode, &QPushButton::clicked, this, &MainWindow::showPseudocode);
}

void MainWindow::calculateWear()
{
    QString partName = partSelector->currentText();
    int lastService = lastReplacedInput->text().toInt();
    int currentOdo = currentKmInput->text().toInt();

    int lifespan = partDatabase.value(partName);

    if(lifespan == 0 || currentOdo < lastService) return;

    int kmDriven = currentOdo - lastService;
    double wearRatio = (double)kmDriven / lifespan;
    int wearPercent = wearRatio * 100;

    partsList.append({partName, lastService, lifespan});

    int row = partsTable->rowCount();
    partsTable->insertRow(row);
    partsTable->setItem(row, 0, new QTableWidgetItem(partName));
    partsTable->setItem(row, 1, new QTableWidgetItem(QString::number(lifespan) + " km"));

    QTableWidgetItem *statusItem = new QTableWidgetItem();
    QTableWidgetItem *actionItem = new QTableWidgetItem();
    QTableWidgetItem *wearItem = new QTableWidgetItem(QString::number(wearPercent) + "%");

    if (wearPercent >= 100) {
        statusItem->setText("REPLACE");
        statusItem->setBackground(Qt::red);
        statusItem->setForeground(Qt::white);
        actionItem->setText("Service Required");
    } else {
        statusItem->setText("OK");
        statusItem->setBackground(Qt::green);
        statusItem->setForeground(Qt::black);
        actionItem->setText("None");
    }

    partsTable->setItem(row, 2, statusItem);
    partsTable->setItem(row, 3, wearItem);
    partsTable->setItem(row, 4, actionItem);

    updateGraph();

    lastReplacedInput->clear();
}

void MainWindow::updateGraph()
{
    series->clear();
    QStringList categories;

    QBarSet *set = new QBarSet("Wear Level");

    int currentOdo = currentKmInput->text().toInt();
    if (currentOdo == 0 && !partsList.isEmpty()) {
        currentOdo = partsList.last().lastReplacedKm +
                     (partsList.last().lifeSpanKm * 0.5);
    }

    for(const auto &part : partsList) {
        int kmDriven = currentOdo - part.lastReplacedKm;
        double wear = ((double)kmDriven / part.lifeSpanKm) * 100.0;

        if(wear > 100.0) wear = 100.0;

        *set << wear;
        categories << part.name;
    }

    series->append(set);

    if (axisX) {
        chart->removeAxis(axisX);
        delete axisX;
    }
    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
}

void MainWindow::showPseudocode()
{
    QString code =
        "START PROCEDURE CheckPartStatus\n"
        "   INPUT: SelectedPart, LastServiceKm, CurrentKm\n"
        "   LOOKUP: PartLifespan FROM Database WHERE Part = SelectedPart\n"
        "   \n"
        "   CALCULATE KmDriven = CurrentKm - LastServiceKm\n"
        "   CALCULATE WearPercent = (KmDriven / PartLifespan) * 100\n"
        "   \n"
        "   IF WearPercent >= 100 THEN\n"
        "       DISPLAY 'Status: REPLACE' (Color: RED)\n"
        "   ELSE\n"
        "       DISPLAY 'Status: OK' (Color: GREEN)\n"
        "   END IF\n"
        "   \n"
        "   PLOT Graph (Max Y-Axis = 100%)\n"
        "END PROCEDURE";

    QMessageBox::information(this, "Application Logic", code);
}
