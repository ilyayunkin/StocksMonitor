#ifndef MODELSREFERENCE_H
#define MODELSREFERENCE_H

#include <memory>
#include <vector>

#include <QString>

class AbstractStocksModel;
class StocksLimitsDatabase;

struct ModelsReference
{
    QString name;
    QByteArray time;
    std::shared_ptr<AbstractStocksModel> stocksModel;
    std::shared_ptr<StocksLimitsDatabase> limitsModel;
};

typedef std::vector<ModelsReference> ModelsReferenceList;

#endif // MODELSREFERENCE_H
