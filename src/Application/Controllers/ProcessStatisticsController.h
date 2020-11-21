#ifndef ProcessStatisticsController_H
#define ProcessStatisticsController_H

class ProcessStatisticsInteractor;
class AbstractStatisticsView;

class ProcessStatisticsController
{
public:
    ProcessStatisticsController(ProcessStatisticsInteractor &rules,
                         AbstractStatisticsView &resultView);
    void processStatistics() const;
private:
    ProcessStatisticsInteractor &rules;
    AbstractStatisticsView &resultView;
};

#endif // ProcessStatisticsController_H
