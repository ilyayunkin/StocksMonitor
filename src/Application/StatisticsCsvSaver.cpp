#include "StatisticsCsvSaver.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QLocale>
#include <QDesktopServices>

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
            if(f.open(QIODevice::WriteOnly))
            {
                QLocale locale(QLocale::Russian, QLocale::Russia);
                QTextStream stream(&f);
                constexpr auto separator = ';';
                for(auto const &counter : statistics.list)
                {
                    stream << counter.name << '\n';;
                    stream << QObject::tr("Group")
                           << separator << QObject::tr("Sum")
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
            }
        }
        openFileInDefaultApp(filename);
    }
}
