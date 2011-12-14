#include "roomcontrolwidget.h"


RoomControlWidget::RoomControlWidget(QWidget *parent, int roomNumber) :
    QWidget(parent)
{
    m_iRoomNumber = roomNumber;    
    setupLabels();
    setupFunctionalWidgets();
    setupWidgetsAndLayout();
    setupRoomConstraints();
    setupConnections();
}

void RoomControlWidget::setupConnections()
{
    connect(m_VLEDSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderLEDChanged(int)));
    connect(m_VFANSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderFANChanged(int)));
    connect(m_HeaterCheckBox, SIGNAL(clicked(bool)), this, SLOT(heaterCheckBoxChanged(bool)));
    connect(m_TempSpinBox, SIGNAL(valueChanged(double)), this, SLOT(autoTempChanged(double)));
    connect(m_BrightnessSpinBox, SIGNAL(valueChanged(int)), this, SLOT(autoBrightnessChanged(int)));
}


void RoomControlWidget::setupLabels()
{
    QString roomName("Room ");
    roomName.append(QString::number(m_iRoomNumber));
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
    m_VLEDSlider->setTracking(true);

    m_VFANSlider = new QSlider(Qt::Vertical, this);
    m_VFANSlider->setRange(0, 100);
    m_VFANSlider->setSingleStep(1);
    m_VFANSlider->setTracking(true);

    QString heaterText("Heater");
    m_HeaterCheckBox = new QCheckBox(heaterText, this);

    m_TempSpinBox = new QDoubleSpinBox(this);
    m_TempSpinBox->setDecimals(1);
    m_TempSpinBox->setRange(5.0, 40.0);
    m_TempSpinBox->setSingleStep(0.5);
    m_TempSpinBox->setValue(22.0);

    m_BrightnessSpinBox = new QSpinBox(this);
    m_BrightnessSpinBox->setRange(0, 100);
    m_BrightnessSpinBox->setSingleStep(5);
    m_BrightnessSpinBox->setValue(50);

    m_LEDFANControlsGrid = new QGridLayout(this);

    m_LEDFANControlWidget = new QWidget(this);

}

void RoomControlWidget::setupWidgetsAndLayout()
{
    m_VBoxLayout = new QVBoxLayout(this);
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

    setLayout(m_VBoxLayout);
}

void RoomControlWidget::automodeChanged(bool newAutomode)
{
    m_VLEDSlider->setEnabled(!newAutomode);

    if(m_iRoomNumber == 2 || m_iRoomNumber == 4)
        m_VFANSlider->setEnabled(!newAutomode);
    if(m_iRoomNumber != 1) {
        m_HeaterCheckBox->setEnabled(!newAutomode);
    }
}

void RoomControlWidget::setupRoomConstraints()
{
    switch(m_iRoomNumber) {
    case 1:
        m_VFANSlider->setDisabled(true);
        m_HeaterCheckBox->setDisabled(true);
        break;
    case 3:
        m_VFANSlider->setDisabled(true);
        break;
    default:
        // No changes from origin
        break;
    }
}

void RoomControlWidget::heaterCheckBoxChanged(bool newValue)
{
    sharedData->setHeaterInRoom(m_iRoomNumber, newValue);
}

void RoomControlWidget::sliderLEDChanged(int newValue)
{
    sharedData->setLedInRoom(m_iRoomNumber, newValue);
}

void RoomControlWidget::sliderFANChanged(int newValue)
{
    sharedData->setFanInRoom(m_iRoomNumber, newValue);
}

void RoomControlWidget::autoTempChanged(double newValue)
{
    sharedData->setWantedTempInRoom(m_iRoomNumber, newValue);
}

void RoomControlWidget::autoBrightnessChanged(int newValue)
{
    sharedData->setWantedBrightnessInRoom(m_iRoomNumber, newValue);
}

void RoomControlWidget::dataChanged(int type)
{
    switch(type) {
    case 0:
        m_TempSpinBox->blockSignals(true);
        m_TempSpinBox->setValue(sharedData->getWantedTempInRoom(m_iRoomNumber));
        m_TempSpinBox->blockSignals(false);
        break;
    case 1:
        m_BrightnessSpinBox->blockSignals(true);
        m_BrightnessSpinBox->setValue(sharedData->getWantedBrightnessInRoom(m_iRoomNumber));
        m_BrightnessSpinBox->blockSignals(false);
        break;
    case 2:
        m_VLEDSlider->blockSignals(true);
        m_VLEDSlider->setValue(sharedData->getLedInRoom(m_iRoomNumber));
        m_VLEDSlider->blockSignals(false);
        break;
    case 3:
        m_VFANSlider->blockSignals(true);
        m_VFANSlider->setValue(sharedData->getFanInRoom(m_iRoomNumber));
        m_VFANSlider->blockSignals(false);
        break;
    case 4:
        m_HeaterCheckBox->blockSignals(true);
        m_HeaterCheckBox->setChecked(sharedData->getHeaterInRoom(m_iRoomNumber));
        m_HeaterCheckBox->blockSignals(false);
        break;
    }
}
