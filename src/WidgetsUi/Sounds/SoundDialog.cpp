#include "SoundDialog.h"
#include "ui_SoundDialog.h"

#include <QFileDialog>

SoundDialog::SoundDialog(const QString dir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SoundDialog),
    dir(dir)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
}

SoundDialog::~SoundDialog()
{
    delete ui;
}

QString SoundDialog::getSoundFileName(const QString dir)
{
    SoundDialog d(dir);
    if(d.exec() == QDialog::Accepted){
        return d.sound->fileName();
    }
    return QString();
}

void SoundDialog::on_selectButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select sound"), dir, "*.wav");
    ui->fileNameEdit->setText(filename);
    setupFile(filename);
}

void SoundDialog::on_playButton_clicked()
{
    sound->play();
}

void SoundDialog::on_stopButton_clicked()
{
    sound->stop();
}

void SoundDialog::setupFile(const QString &filename)
{
    if(!filename.isEmpty()){
        sound = std::make_unique<QSound>(filename);
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->okButton->setEnabled(true);
    }
}
