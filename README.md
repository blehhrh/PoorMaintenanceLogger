# PoorMaintenanceLogger
This is a C++ desktop application built with the Qt Framework designed to help vehicle owners track component wear and maintenance schedules. The application replaces manual logging by digitizing the process:
- Input: Users enter the "Current Odometer" and "Last Service Odometer."
- Calculation: The app compares the distance driven against a built-in database of industry-standard service intervals (e.g., Synthetic Oil = 10,000 km, Timing Belt = 100,000 km).
- Visualization: It provides immediate color-coded feedback (Green for OK, Red for Replace) and displays a dynamic bar chart of component health.

# Prerequisits
To build and run this project, ensure you have the following installed:
- Qt Creator
- Qt Framework (Qt 5.15 or Qt 6.x).
- Qt Charts Module (Essential for the graphing feature).

# Usage
1.Launch Qt Creator - Open the MaintenanceLogger.pro file.

2.Build and Run - Click the Run button (Green Arrow) or press Ctrl+R (Windows/Linux) / Cmd+R (macOS).
