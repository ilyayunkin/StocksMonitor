#ifndef MODELSREFERENCE_H
#define MODELSREFERENCE_H

#include <memory>
#include <vector>

#include <QString>

class StocksModel;
class StocksLimitsModel;

struct ModelsReference
{
    QString name;
    QByteArray time;
    std::shared_ptr<StocksModel> stocksModel;
    std::shared_ptr<StocksLimitsModel> limitsModel;
};

typedef std::vector<ModelsReference> ModelsReferenceList;

#endif // MODELSREFERENCE_H
