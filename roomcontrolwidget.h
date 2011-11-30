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

class RoomControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomControlWidget(QWidget *parent = 0, int roomNumber = 0);
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

    void setupLabels();
    void setupFunctionalWidgets();
    void setupConnections();
    void addWidgetsToLayout();
    void fixRoomConstraints();

signals:
    void heaterCheckBoxChanged(int newState); // 0 = Unchecked, 2 = Checked
    void sliderLEDChanged(int newValue);
    void sliderFANChanged(int newValue);
    void autoTempChanged(double newValue);
    void autoBrightnessChanged(int newValue);

public slots:
    void automodeChanged(bool newAutomode);

};

#endif // ROOMCONTROLWIDGET_H
