#include "Signalizer.h"

#include <QDebug>
#include <QSettings>

#include "SoundDialog.h"

void Signalizer::setupFile(const QString &filename)
{
    if(!filename.isEmpty()){
        sound = std::make_shared<QSound>(filename);
    }
}

Signalizer::Signalizer()
{
    QSettings settings;
    QString filename = settings.value("sound").toString();
    qDebug() << filename;
    setupFile(filename);
}

void Signalizer::signalize()
{
    if(sound.get() != nullptr){
        sound->play();
    }
}

void Signalizer::changeSound()
{
    QString filename = SoundDialog::getSoundFileName();
    qDebug() << filename;
    setupFile(filename);
    if(!filename.isEmpty())
    {
        QSettings settings;
        settings.setValue("sound", filename);
    }
}
