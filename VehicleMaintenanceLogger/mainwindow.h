#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QtCharts>
#include <QVBoxLayout>
#include <QMap>

struct VehiclePart {
    QString name;
    int lastReplacedKm;
    int lifeSpanKm;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calculateWear();
    void showPseudocode();
    void updateGraph();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QComboBox *partSelector;
    QLineEdit *lastReplacedInput;
    QLineEdit *currentKmInput;

    QTableWidget *partsTable;
    QPushButton *btnCalculate;
    QPushButton *btnPseudocode;

    QChartView *chartView;
    QBarSeries *series;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

    QList<VehiclePart> partsList;
    QMap<QString, int> partDatabase;

    void setupUI();
    void initPartDatabase();
};

#endif
