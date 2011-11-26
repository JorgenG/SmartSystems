#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupWidgets();
    configureTabWidget();


    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupWidgets()
{
    centralWidget = new QWidget(this);
    centralLayout = new QGridLayout(this);

    tabWidget = new QTabWidget(this);
    sensorDataTreeWidget = new QTreeWidget(this);
    rightWidgetContainer = new QWidget(this);
    rightWidgetLayout = new QVBoxLayout();

    roomControlGroupBox = new QGroupBox(this);
    roomControlGroupBoxLayout = new QHBoxLayout();
    buttonWidgetContainer = new QWidget(this);
    buttonWidgetLayout = new QHBoxLayout();

    roomControlWidgets = new RoomControlWidget*[4];
    for(int i = 0; i < 4; i++) {
        roomControlWidgets[i] = new RoomControlWidget(this, i+1);
    }

    toggleAutoModeButton = new QPushButton(this);
    startButton = new QPushButton(this);
    exitButton = new QPushButton(this);
}

void MainWindow::configureTreeWidget()
{

}

void MainWindow::addWidgetsToLayout()
{
    centralLayout->addWidget();
    centralWidget->setLayout(centralLayout);
}
