#include "StatisticsCsvSaver.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QLocale>
#include <QDesktopServices>

#include "ExceptionClasses.h"

#include <algorithm>

namespace  {
void openFileInDefaultApp(QString const &filename)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}
}

StatisticsCsvSaver::StatisticsCsvSaver()
{

}

void StatisticsCsvSaver::show(const Statistics &statistics)
{
    const auto filename = QFileDialog::getSaveFileName(nullptr, QObject::tr("Select file"), QString(), "*.csv");
    if(!filename.isEmpty())
    {
        {
            QFile f(filename);
            if(!f.open(QIODevice::WriteOnly))
                throw CantOpenFileException();

            QLocale locale(QLocale::Russian, QLocale::Russia);
            QTextStream stream(&f);
            constexpr auto separator = ';';
            for(auto const &counter : statistics.list)
            {
                stream << counter.name << '\n';;
                stream << QObject::tr("Group")
                       << separator << QObject::tr("%")
                       << separator << QObject::tr("Sum, RUB")
                       << separator << QObject::tr("~ Day, RUB")
                       << separator << QObject::tr("~ Week, RUB")
                       << separator << QObject::tr("~ Month, RUB")
                       << separator << QObject::tr("~ Year, RUB")
                       << separator << QObject::tr("~ Day, %")
                       << separator << QObject::tr("~ Week, %")
                       << separator << QObject::tr("~ Month, %")
                       << separator << QObject::tr("~ Year, %")
                       << separator << QObject::tr("Tickers")
                       << '\n';
                for(auto const &group : counter.list)
                {
                    QStringList stocks;
                    std::transform(group.tickers.begin(),
                                   group.tickers.end(),
                                   std::back_inserter(stocks),
                                   [](const auto &stock){return stock.data();});

                    stream << group.name
                           << separator << locale.toString(group.fractionPercent)
                           << separator << locale.toString(group.totalSum)
                           << separator << locale.toString(group.totalDerivation)
                           << separator << locale.toString(group.totalDerivationWeek)
                           << separator << locale.toString(group.totalDerivationMonth)
                           << separator << locale.toString(group.totalDerivationYear)
                           << separator << locale.toString(group.totalDerivationPercent)
                           << separator << locale.toString(group.totalDerivationWeekPercent)
                           << separator << locale.toString(group.totalDerivationMonthPercent)
                           << separator << locale.toString(group.totalDerivationYearPercent)
                           << separator << stocks.join(", ")
                           << '\n';
                }
                stream << QObject::tr("Total")
                       << separator
                       << separator << locale.toString(statistics.totalSum)
                       << separator << locale.toString(statistics.totalDerivation)
                       << separator << locale.toString(statistics.totalDerivationWeek)
                       << separator << locale.toString(statistics.totalDerivationMonth)
                       << separator << locale.toString(statistics.totalDerivationYear)
                       << separator << locale.toString(statistics.totalDerivationPercent)
                       << separator << locale.toString(statistics.totalDerivationWeekPercent)
                       << separator << locale.toString(statistics.totalDerivationMonthPercent)
                       << separator << locale.toString(statistics.totalDerivationYearPercent)
                       << '\n'
                       << '\n';
            }
        }
        openFileInDefaultApp(filename);
    }
}
