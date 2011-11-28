#include "roomcontrolwidget.h"


RoomControlWidget::RoomControlWidget(QWidget *parent, int roomNumber) :
    QWidget(parent)
{
    m_iRoomNumber = roomNumber;
    m_VBoxLayout = new QVBoxLayout(this);
    setupLabels();
    setupFunctionalWidgets();
    addWidgetsToLayout();
    setLayout(m_VBoxLayout);
    setupConnections();
}

void RoomControlWidget::setupConnections()
{
    connect(m_VLEDSlider, SIGNAL(valueChanged(value)), this, SIGNAL(sliderLEDChanged(value)));
    connect(m_VFANSlider, SIGNAL(valueChanged(value)), this, SIGNAL(sliderFANChanged(value)));
    connect(m_HeaterCheckBox, SIGNAL(stateChanged(value)), this, SIGNAL(heaterCheckBoxChanged(value)));
    connect(m_TempSpinBox, SIGNAL(valueChanged(value)), this, SIGNAL(autoTempChanged(value)));
    connect(m_BrightnessSpinBox, SIGNAL(valueChanged(value)), this, SIGNAL(autoBrightnessChanged(value)));
}


void RoomControlWidget::setupLabels()
{
    QString roomName("Room ");
    QString roomNumber;
    roomNumber.setNum(m_iRoomNumber);
    roomName.append(roomNumber);
    roomName.append(":");
    m_RoomTitleLabel = new QLabel(roomName, this);

    QString autoTempText("Temp(Auto)");
    m_TempTextLabel = new QLabel(autoTempText, this);

    QString autoBrightnessText("Brightness (%)");
    m_BrightnessTextLabel = new QLabel(autoBrightnessText, this);

    QString ledText("LED");
    m_LEDLabel = new QLabel(ledText, this);

    QString fanText("FAN");
    m_FANLabel = new QLabel(fanText, this);
}

void RoomControlWidget::setupFunctionalWidgets()
{
    m_VLEDSlider = new QSlider(Qt::Vertical, this);
    m_VLEDSlider->setRange(0, 100);
    m_VLEDSlider->setSingleStep(1);

    m_VFANSlider = new QSlider(Qt::Vertical, this);
    m_VFANSlider->setRange(0, 100);
    m_VFANSlider->setSingleStep(1);

    QString heaterText("Heater");
    m_HeaterCheckBox = new QCheckBox(heaterText, this);

    m_TempSpinBox = new QDoubleSpinBox(this);
    m_TempSpinBox->setDecimals(1);
    m_TempSpinBox->setRange(5.0, 40.0);
    m_TempSpinBox->setSingleStep(0.5);
    m_TempSpinBox->setValue(22.0);
    m_TempSpinBox->setEnabled(false);

    m_BrightnessSpinBox = new QSpinBox(this);
    m_BrightnessSpinBox->setRange(0, 100);
    m_BrightnessSpinBox->setSingleStep(5);
    m_BrightnessSpinBox->setValue(50);
    m_BrightnessSpinBox->setEnabled(false);

    m_LEDFANControlsGrid = new QGridLayout(this);

    m_LEDFANControlWidget = new QWidget(this);

}

void RoomControlWidget::addWidgetsToLayout()
{
    m_VBoxLayout->addWidget(m_RoomTitleLabel, 0, Qt::AlignCenter);

    m_LEDFANControlsGrid->addWidget(m_LEDLabel, 0, 0, Qt::AlignRight);
    m_LEDFANControlsGrid->addWidget(m_FANLabel, 0, 1, Qt::AlignLeft);
    m_LEDFANControlsGrid->addWidget(m_VLEDSlider, 1, 0, Qt::AlignRight);
    m_LEDFANControlsGrid->addWidget(m_VFANSlider, 1, 1, Qt::AlignLeft);
    m_LEDFANControlWidget->setLayout(m_LEDFANControlsGrid);
    m_VBoxLayout->addWidget(m_LEDFANControlWidget);

    m_VBoxLayout->addWidget(m_HeaterCheckBox, 0, Qt::AlignCenter);
    m_VBoxLayout->addWidget(m_TempTextLabel, 0, Qt::AlignCenter);
    m_VBoxLayout->addWidget(m_TempSpinBox, 0, Qt::AlignCenter);
    m_VBoxLayout->addWidget(m_BrightnessTextLabel, 0, Qt::AlignCenter);
    m_VBoxLayout->addWidget(m_BrightnessSpinBox, 0, Qt::AlignCenter);
}

void RoomControlWidget::automodeChanged(bool newAutomode)
{
    m_BrightnessSpinBox->setEnabled(newAutomode);
    m_TempSpinBox->setEnabled(newAutomode);
    m_VLEDSlider->setEnabled(!newAutomode);
    m_VFANSlider->setEnabled(!newAutomode);
    m_HeaterCheckBox->setEnabled(!newAutomode);
}
