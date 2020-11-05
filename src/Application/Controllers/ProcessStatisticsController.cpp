#include "ProcessStatisticsController.h"
#include "Rules/ProcessStatisticsInteractor.h"
#include "Application/AbstractStatisticsView.h"

#include "ExceptionClasses.h"

#include <QMessageBox>

ProcessStatisticsController::ProcessStatisticsController(ProcessStatisticsInteractor &rules,
                                           AbstractStatisticsView &view)
    : rules(rules)
    , resultView(view)
{
}

void ProcessStatisticsController::processStatistics() const
{
    try {
        resultView.show(rules.processStatistics());
    } catch (EmptyPortfolioException &e) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Portfolio if empty."),
                              QObject::tr("Nothing to analyze."));
    } catch (CantOpenFileException &e) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Can't open the file."),
                              QObject::tr("Can't open the file."));
    }
}
