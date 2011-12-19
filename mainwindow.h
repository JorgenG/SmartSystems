#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGridLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <roomcontrolwidget.h>
#include <QtGlobal>
#include <QLine>
#include <QTreeWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QListWidget>

#include "logger.h"
#include "shareddata/shareddata.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    RoomControlWidget* *roomControlWidgets;
    QWidget *centralWidget, *controlAndMonitorTab, *rightWidgetContainer, *buttonWidgetContainer, *logTab;
    QGridLayout *centralLayout, *controlAndMonitorTabLayout, *logTabLayout;
    QGroupBox *roomControlGroupBox;
    QTreeWidget *sensorDataTreeWidget;
    QTabWidget *tabWidget;
    QVBoxLayout *rightWidgetLayout;
    QHBoxLayout *buttonWidgetLayout, *roomControlGroupBoxLayout;
    QPushButton *toggleAutoModeButton, *startButton, *exitButton;
    QListWidget *logListWidget;
    bool automode;

    /**
      Setup of all widgets added.
      */
    void setupWidgets();

    /**
      Configures the tree widget to suit our needs*
      */
    void configureTreeWidget();

    /**
      Adds all the widgets to the layout
      */
    void addWidgetsToLayout();

public slots:
    /**
      This slot is signaled when the sensor data should be updated in the GUI
      */
    void updateSensorData();

    /**
      This slot is signaled when a log entry is added,
      is used for updating the log list window.
      */
    void logEntryAdded();

    /**
      This slot is signaled when data is changed from the web interface,
      this is to update the actual GUI to correspond to the webinterface.
      */
    void dataChangedInRoomFromWeb(int room, int type);

    /**
      This slot is signaled when automode is changed. This toggels various
      component which arent usable during regulation.
      */
    void autoModeChanged(bool newValue);

signals:
    /**
      This signals when the exit button is clicked. This signal is connected
      to the shutdown procedure of the application.
      */
    void exitButtonClicked();

};

#endif // MAINWINDOW_H
