#ifndef ROOMCONTROLWIDGET_H
#define ROOMCONTROLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QSpinBox>

#include "shareddata/shareddata.h"
#include "logger.h"

class RoomControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomControlWidget(QWidget *parent = 0, int roomNumber = 0);
    void dataChanged(int type);
private:
    int m_iRoomNumber;
    QLabel *m_RoomTitleLabel, *m_TempTextLabel, *m_BrightnessTextLabel, *m_LEDLabel, *m_FANLabel;
    QVBoxLayout *m_VBoxLayout;
    QGridLayout *m_LEDFANControlsGrid;
    QWidget *m_LEDFANControlWidget;
    QCheckBox *m_HeaterCheckBox;
    QDoubleSpinBox *m_TempSpinBox;
    QSpinBox *m_BrightnessSpinBox;
    QSlider *m_VLEDSlider, *m_VFANSlider;

    /**
      Sets up the different labels used in the roomwidget
      */
    void setupLabels();

    /**
      Sets up all the functional widgets in the roomwidget
      */
    void setupFunctionalWidgets();

    /**
      Setup of connections
      */
    void setupConnections();

    /**
      Setup of layouts, and adding of the different widgets
      */
    void setupWidgetsAndLayout();

    /**
      Adds the room specific constraints.
      */
    void setupRoomConstraints();

public slots:
    /**
      This slot is signaled by a change in automode.
      */
    void automodeChanged(bool newAutomode);

    /**
      This slot is signaled by change in the heatercheckbox
      */
    void heaterCheckBoxChanged(bool newValue);

    /**
      This slot is signaled when the LED Slider is changed
      */
    void sliderLEDChanged(int newValue);

    /**
      This slot is signaled when the FAN Slider is changed
      */
    void sliderFANChanged(int newValue);

    /**
      This slot is signaled when the wanted temperature is changed
      */
    void autoTempChanged(double newValue);

    /**
      This slot is signaled when wanted brightness is changed.
      */
    void autoBrightnessChanged(int newValue);

};

#endif // ROOMCONTROLWIDGET_H
