#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupWidgets();
    configureTreeWidget();
    addWidgetsToLayout();
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupWidgets()
{
    centralWidget = new QWidget(this);
    centralLayout = new QGridLayout(centralWidget);

    tabWidget = new QTabWidget(centralWidget);
    controlAndMonitorTab = new QWidget(tabWidget);
    controlAndMonitorTabLayout = new QGridLayout(controlAndMonitorTab);

    sensorDataTreeWidget = new QTreeWidget(tabWidget);
    rightWidgetContainer = new QWidget(tabWidget);
    rightWidgetLayout = new QVBoxLayout(rightWidgetContainer);

    roomControlGroupBox = new QGroupBox(rightWidgetContainer);
    roomControlGroupBoxLayout = new QHBoxLayout(roomControlGroupBox);
    buttonWidgetContainer = new QWidget(rightWidgetContainer);
    buttonWidgetLayout = new QHBoxLayout(buttonWidgetContainer);

    roomControlWidgets = new RoomControlWidget*[4];
    for(int i = 0; i < 4; i++) {
        roomControlWidgets[i] = new RoomControlWidget(roomControlGroupBox, i+1);
    }

    toggleAutoModeButton = new QPushButton("Toggle AutoMode", buttonWidgetContainer);
    startButton = new QPushButton("Start", buttonWidgetContainer);
    exitButton = new QPushButton("Exit", buttonWidgetContainer);
}

void MainWindow::configureTreeWidget()
{

}

void MainWindow::addWidgetsToLayout()
{
    for(int i = 0; i < 4; i++) {
        roomControlGroupBoxLayout->addWidget(roomControlWidgets[i]);
    }
    roomControlGroupBox->setLayout(roomControlGroupBoxLayout);

    buttonWidgetLayout->addWidget(toggleAutoModeButton);
    buttonWidgetLayout->addWidget(startButton);
    buttonWidgetLayout->addWidget(exitButton);
    buttonWidgetContainer->setLayout(buttonWidgetLayout);

    rightWidgetLayout->addWidget(roomControlGroupBox);
    rightWidgetLayout->addWidget(buttonWidgetContainer);
    rightWidgetContainer->setLayout(rightWidgetLayout);

    controlAndMonitorTabLayout->addWidget(sensorDataTreeWidget, 0, 0, Qt::AlignLeft);
    controlAndMonitorTabLayout->addWidget(rightWidgetContainer, 0, 1, Qt::AlignRight);
    controlAndMonitorTab->setLayout(controlAndMonitorTabLayout);

    tabWidget->addTab(controlAndMonitorTab, "Monitor & Control");
    centralLayout->addWidget(tabWidget, 0, 0);
    centralWidget->setLayout(centralLayout);
}
