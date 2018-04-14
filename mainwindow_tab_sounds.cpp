#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - SOUND TAB
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::SetupControls_SoundTab(void)
{
    // Array of squeak controls
    chkSqueakEnabled[0] = ui->chkEnableSqueak1;
    chkSqueakEnabled[1] = ui->chkEnableSqueak2;
    chkSqueakEnabled[2] = ui->chkEnableSqueak3;
    chkSqueakEnabled[3] = ui->chkEnableSqueak4;
    chkSqueakEnabled[4] = ui->chkEnableSqueak5;
    chkSqueakEnabled[5] = ui->chkEnableSqueak6;

    squeakMinInterval[0] = ui->spinSqueak1Min;
    squeakMinInterval[1] = ui->spinSqueak2Min;
    squeakMinInterval[2] = ui->spinSqueak3Min;
    squeakMinInterval[3] = ui->spinSqueak4Min;
    squeakMinInterval[4] = ui->spinSqueak5Min;
    squeakMinInterval[5] = ui->spinSqueak6Min;

    squeakMaxInterval[0] = ui->spinSqueak1Max;
    squeakMaxInterval[1] = ui->spinSqueak2Max;
    squeakMaxInterval[2] = ui->spinSqueak3Max;
    squeakMaxInterval[3] = ui->spinSqueak4Max;
    squeakMaxInterval[4] = ui->spinSqueak5Max;
    squeakMaxInterval[5] = ui->spinSqueak6Max;

    // Connect Signals and Slots
    // If we disable one of the squeaks, we also disable the min/max settings for that squeak
    connect(ui->chkEnableSqueak1, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak1Settings(bool)));
    connect(ui->chkEnableSqueak2, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak2Settings(bool)));
    connect(ui->chkEnableSqueak3, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak3Settings(bool)));
    connect(ui->chkEnableSqueak4, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak4Settings(bool)));
    connect(ui->chkEnableSqueak5, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak5Settings(bool)));
    connect(ui->chkEnableSqueak6, SIGNAL(clicked(bool)), this, SLOT(ShowHideSqueak6Settings(bool)));
    // Also, if we disable all three squeaks, no need to show the min-squeak speed or any other generic squeak settings
    connect(ui->chkEnableSqueak1, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));
    connect(ui->chkEnableSqueak2, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));
    connect(ui->chkEnableSqueak3, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));
    connect(ui->chkEnableSqueak4, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));
    connect(ui->chkEnableSqueak5, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));
    connect(ui->chkEnableSqueak6, SIGNAL(clicked()), this, SLOT(ShowHideOtherSqueakSettings()));

    // Apply css to the volume sliders
    QFile file(":/css/volumeslider.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->sliderEngineVolume->setStyleSheet(file.readAll());
        file.seek(0);   // Return to beginning of file
        ui->sliderOverlayVolume->setStyleSheet(file.readAll());
        file.seek(0);   // Return to beginning of file
        ui->sliderEffectsVolume->setStyleSheet(file.readAll());
        file.close();
    }

    // Signals and slots for volume sliders
    connect(ui->sliderEngineVolume, SIGNAL(valueChanged(int)), this, SLOT(UpdateEngineVolumeLabel(int)));
    connect(ui->sliderOverlayVolume, SIGNAL(valueChanged(int)), this, SLOT(UpdateOverlayVolumeLabel(int)));
    connect(ui->sliderEffectsVolume, SIGNAL(valueChanged(int)), this, SLOT(UpdateEffectsVolumeLabel(int)));

}

void MainWindow::UpdateEngineVolumeLabel(int v)
{
    ui->lblEngineVolume->setText(QString("%1%").arg(v));
}

void MainWindow::UpdateOverlayVolumeLabel(int v)
{
    ui->lblOverlayVolume->setText(QString("%1%").arg(v));
}

void MainWindow::UpdateEffectsVolumeLabel(int v)
{
    ui->lblEffectsVolume->setText(QString("%1%").arg(v));
}

void MainWindow::ShowHideSqueak1Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak1Min->setEnabled(true);
        ui->spinSqueak1Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak1Min->setEnabled(false);
        ui->spinSqueak1Max->setEnabled(false);
    }
}
void MainWindow::ShowHideSqueak2Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak2Min->setEnabled(true);
        ui->spinSqueak2Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak2Min->setEnabled(false);
        ui->spinSqueak2Max->setEnabled(false);
    }
}
void MainWindow::ShowHideSqueak3Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak3Min->setEnabled(true);
        ui->spinSqueak3Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak3Min->setEnabled(false);
        ui->spinSqueak3Max->setEnabled(false);
    }
}
void MainWindow::ShowHideSqueak4Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak4Min->setEnabled(true);
        ui->spinSqueak4Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak4Min->setEnabled(false);
        ui->spinSqueak4Max->setEnabled(false);
    }
}
void MainWindow::ShowHideSqueak5Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak5Min->setEnabled(true);
        ui->spinSqueak5Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak5Min->setEnabled(false);
        ui->spinSqueak5Max->setEnabled(false);
    }
}
void MainWindow::ShowHideSqueak6Settings(bool isChecked)
{
    if (isChecked)
    {
        ui->spinSqueak6Min->setEnabled(true);
        ui->spinSqueak6Max->setEnabled(true);
    }
    else
    {
        ui->spinSqueak6Min->setEnabled(false);
        ui->spinSqueak6Max->setEnabled(false);
    }
}
void MainWindow::ShowHideOtherSqueakSettings(void)
{
    // If any squeak is enabled, enable general squeak settings
    if (ui->chkEnableSqueak1->isChecked() ||
            ui->chkEnableSqueak2->isChecked() ||
            ui->chkEnableSqueak3->isChecked() ||
            ui->chkEnableSqueak4->isChecked() ||
            ui->chkEnableSqueak5->isChecked() ||
            ui->chkEnableSqueak6->isChecked() )
    {
        ui->lblMinSqueakSpeed1->setEnabled(true);
        ui->lblMinSqueakSpeed2->setEnabled(true);
        ui->spinMinSqueakSpeed->setEnabled(true);
        ui->spinMinSqueakSpeed->setValue(DeviceData.MinSqueakSpeedPct);
    }
    else
    {
        // If all squeaks are disabled, disable general squeak settings
        ui->lblMinSqueakSpeed1->setEnabled(false);
        ui->lblMinSqueakSpeed2->setEnabled(false);
        ui->spinMinSqueakSpeed->setValue(DeviceData.MinSqueakSpeedPct);
        ui->spinMinSqueakSpeed->setEnabled(false);
    }
}



