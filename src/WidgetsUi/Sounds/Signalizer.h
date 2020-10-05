#ifndef SIGNALIZER_H
#define SIGNALIZER_H

#include "AbstractSignalizer.h"

#include <QSound>

#include <memory>

class Signalizer : public QObject, public AbstractSignalizer
{
    Q_OBJECT

    std::shared_ptr<QSound> sound;
    void setupFile(const QString &filename);
public:
    explicit Signalizer(QObject *parent = nullptr);

    // AbstractSignalizer interface
public:
    void signalize() override;
    void changeSound() override;
};

#endif // SIGNALIZER_H
