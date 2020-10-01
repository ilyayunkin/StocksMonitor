#ifndef SIGNALIZER_H
#define SIGNALIZER_H

#include "AbstractSignalizer.h"

#include <QSound>

#include <memory>

class Signalizer : public AbstractSignalizer
{
    std::shared_ptr<QSound> sound;
    void setupFile(const QString &filename);
public:
    Signalizer();

    // AbstractSignalizer interface
public:
    void signalize() override;
    void changeSound() override;
};

#endif // SIGNALIZER_H
