#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupWidgets();    
    addWidgetsToLayout();
    setCentralWidget(centralWidget);
    automode = false;
    connect(exitButton, SIGNAL(clicked()), this, SIGNAL(exitButtonClicked()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(testNewEntry()));
    connect(toggleAutoModeButton, SIGNAL(clicked(bool)), this, SLOT(autoModeChanged(bool)));
    for(int i = 0; i < 4; i++)
        connect(toggleAutoModeButton, SIGNAL(clicked(bool)), roomControlWidgets[i], SLOT(automodeChanged(bool)));
    updateSensorData();
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
    logTab = new QWidget(tabWidget);
    logTabLayout = new QGridLayout(logTab);
    logListWidget = new QListWidget();

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
    toggleAutoModeButton->setCheckable(true);
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
        temperature->setText(1, tr("- °C"));
        QTreeWidgetItem *brightness = new QTreeWidgetItem(rooms);
        brightness->setText(0, tr("Brightness"));
        brightness->setText(1, tr("- %"));
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

    logTabLayout->addWidget(logListWidget);
    logTab->setLayout(logTabLayout);

    tabWidget->addTab(controlAndMonitorTab, "Monitor & Control");
    tabWidget->addTab(logTab, "Log");

    centralLayout->addWidget(tabWidget, 0, 0);
    centralWidget->setLayout(centralLayout);
}

void MainWindow::updateSensorData()
{
    QTreeWidgetItemIterator itemIterator(sensorDataTreeWidget, QTreeWidgetItemIterator::NoChildren);
    double *temperatures = new double[4];
    int *brightness = new int[4];
    sharedData->getSensorData(temperatures, brightness);

    for(int i = 0; i < 4; i++) {
        (*itemIterator++)->setText(1, QString::number(temperatures[i]));
        (*itemIterator++)->setText(1, QString::number(brightness[i]));
    }

    delete temperatures;
    delete brightness;
}

void MainWindow::logEntryAdded()
{
    int totalEntries = logger->getTotalEntries();
    for(int i = logListWidget->count(); i < totalEntries; i++)
    {
        QListWidgetItem *newEntry = new QListWidgetItem(logger->getEntry(i));
        logListWidget->addItem(newEntry);
        logListWidget->setCurrentItem(newEntry);
    }

}

void MainWindow::testNewEntry()
{
    logger->addEntry("Testmessage");
}

void MainWindow::autoModeChanged(bool newValue)
{
    sharedData->setAutomode(newValue);
}

void MainWindow::dataChangedInRoomFromWeb(int room, int type)
{
    // Automode changed from web
    if(room == 0) {
        toggleAutoModeButton->setChecked(sharedData->getAutomode());
        for(int i = 0; i < 4; i++)
            roomControlWidgets[i]->automodeChanged(sharedData->getAutomode());
    }
    else {
        roomControlWidgets[room - 1]->dataChanged(type);
    }
}
