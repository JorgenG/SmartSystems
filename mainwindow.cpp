#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setupWidgets();    
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
    configureTreeWidget();
    rightWidgetContainer = new QWidget(tabWidget);
    rightWidgetLayout = new QVBoxLayout(rightWidgetContainer);

    roomControlGroupBox = new QGroupBox(rightWidgetContainer);
    roomControlGroupBox->setTitle("Control");
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
    QStringList *columnNames = new QStringList("Sensor");
    columnNames->push_back("Values");
    sensorDataTreeWidget->setColumnCount(2);
    sensorDataTreeWidget->setHeaderLabels(*columnNames);
    sensorDataTreeWidget->setColumnWidth(0, 120);
    sensorDataTreeWidget->setColumnWidth(1, 80);
    sensorDataTreeWidget->setAlternatingRowColors(true);

    for(int i = 0; i < 4; i++) {
        QTreeWidgetItem *rooms = new QTreeWidgetItem(sensorDataTreeWidget);
        QString room("Room ");
        QString number;
        number.setNum(i + 1);
        room.append(number);
        rooms->setText(0, room);
        rooms->setExpanded(true);

        QTreeWidgetItem *temperature = new QTreeWidgetItem(rooms);
        temperature->setText(0, tr("Temperature"));
        temperature->setText(1, tr("22 °C"));
        QTreeWidgetItem *brightness = new QTreeWidgetItem(rooms);
        brightness->setText(0, tr("Brightness"));
        brightness->setText(1, tr("50 %"));
    }

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

void MainWindow::updateSensorData()
{
    QTreeWidgetItemIterator itemIterator(sensorDataTreeWidget, QTreeWidgetItemIterator::NoChildren);
    (*itemIterator)->setText(1, "Temp 1");
    itemIterator++;
    (*itemIterator)->setText(1, "Brightness 1");
    itemIterator++;
    (*itemIterator)->setText(1, "Temp 2");
    itemIterator++;
    (*itemIterator)->setText(1, "Brightness 2");
    itemIterator++;
    (*itemIterator)->setText(1, "Temp 3");
    itemIterator++;
    (*itemIterator)->setText(1, "Brightness 3");
    itemIterator++;
    (*itemIterator)->setText(1, "Temp 4");
    itemIterator++;
    (*itemIterator)->setText(1, "Brightness 4");
    itemIterator++;

}
