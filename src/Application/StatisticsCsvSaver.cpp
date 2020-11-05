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
                       << separator << QObject::tr("Sum, RUB")
                       << separator << QObject::tr("%")
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
                           << separator << locale.toString(group.sum)
                           << separator << locale.toString(group.fractionPercent)
                           << separator << stocks.join(", ")
                           << '\n';
                }
                stream << QObject::tr("Total")
                       << separator << locale.toString(statistics.totalSum)
                       << '\n';
                stream << '\n';
            }
            stream << '\n';
            stream << '\n';
            stream << QObject::tr("Total, RUB")
                   << separator << QObject::tr("~ Day, RUB")
                   << separator << QObject::tr("~ Week, RUB")
                   << separator << QObject::tr("~ Month, RUB")
                   << separator << QObject::tr("~ Year, RUB")
                   << '\n';
            stream << statistics.totalSum
                   << separator << locale.toString(statistics.totalDerivation)
                   << separator << locale.toString(statistics.totalDerivationWeek)
                   << separator << locale.toString(statistics.totalDerivationMonth)
                   << separator << locale.toString(statistics.totalDerivationYear)
                   << '\n';
            stream << '\n';
            stream << '\n';
            stream << QObject::tr("Total")
                   << separator << QObject::tr("~ Day, %")
                   << separator << QObject::tr("~ Week, %")
                   << separator << QObject::tr("~ Month, %")
                   << separator << QObject::tr("~ Year, %")
                   << '\n';
            stream << statistics.totalSum
                   << separator << locale.toString(statistics.totalDerivation / (statistics.totalSum - statistics.totalDerivation) * 100)
                   << separator << locale.toString(statistics.totalDerivationWeek / (statistics.totalSum - statistics.totalDerivationWeek) * 100)
                   << separator << locale.toString(statistics.totalDerivationMonth / (statistics.totalSum - statistics.totalDerivationMonth) * 100)
                   << separator << locale.toString(statistics.totalDerivationYear / (statistics.totalSum - statistics.totalDerivationYear) * 100)
                   << '\n';
        }
        openFileInDefaultApp(filename);
    }
}
