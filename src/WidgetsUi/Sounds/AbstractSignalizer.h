#ifndef ABSTRACTSIGNALIZER_H
#define ABSTRACTSIGNALIZER_H

class AbstractSignalizer
{
public:
    virtual void signalize() = 0;
    virtual void changeSound() = 0;
};

#endif // ABSTRACTSIGNALIZER_H
