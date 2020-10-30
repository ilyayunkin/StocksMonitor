#ifndef SIGNALIZER_H
#define SIGNALIZER_H

#include "AbstractSignalizer.h"

#include <QSound>

#include <memory>

class Signalizer : public QObject, public AbstractSignalizer
{
    Q_OBJECT
public:
    explicit Signalizer(QObject *parent = nullptr);

    // AbstractSignalizer interface
    void signalize() override;
    void changeSound() override;
private:
    std::shared_ptr<QSound> sound;
    void setupFile(const QString &filename);
};

#endif // SIGNALIZER_H
