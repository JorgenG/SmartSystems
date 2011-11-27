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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    RoomControlWidget* *roomControlWidgets;
    QWidget *centralWidget, *controlAndMonitorTab, *rightWidgetContainer, *buttonWidgetContainer;
    QGridLayout *centralLayout, *controlAndMonitorTabLayout;
    QGroupBox *roomControlGroupBox;
    QTreeWidget *sensorDataTreeWidget;
    QTabWidget *tabWidget;
    QVBoxLayout *rightWidgetLayout;
    QHBoxLayout *buttonWidgetLayout, *roomControlGroupBoxLayout;
    QPushButton *toggleAutoModeButton, *startButton, *exitButton;

    bool automode;

    void setupWidgets();
    void configureTreeWidget();
    void addWidgetsToLayout();

public slots:
    void updateSensorData();

signals:
    void exitButtonClicked();

};

#endif // MAINWINDOW_H
