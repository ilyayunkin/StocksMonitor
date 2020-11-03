#include "ProcessStatisticsController.h"
#include "Rules/ProcessStatisticsInteractor.h"
#include "Application/AbstractStatisticsView.h"

#include <QMessageBox>

ProcessStatisticsController::ProcessStatisticsController(ProcessStatisticsInteractor &rules,
                                           AbstractStatisticsView &view)
    : rules(rules)
    , resultView(view)
{
}

void ProcessStatisticsController::processStatistics() const
{
    resultView.show(rules.processStatistics());
}
