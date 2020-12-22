#include "CbrfMetalCacheDb.h"
#include <QDebug>
CbrfMetalCacheDb::CbrfMetalCacheDb()
{
}

bool CbrfMetalCacheDb::empty()
{
    return list.empty();
}

std::size_t CbrfMetalCacheDb::size()
{
    return list.size();
}

void CbrfMetalCacheDb::pushNew(const CacheElement &newLine)
{
    if(!cached.date.isValid() || (cached.date < newLine.date))
    {
        OutLine out{newLine.date, cached.list};
        auto findElementInOut = [&](const QByteArray &code){
            return std::find_if(out.list.begin(), out.list.end(),
                                [&](const OutElement &e){return e.code == code;});
        };
        auto outIt = findElementInOut(newLine.code);
        if(outIt != out.list.end()){
            (*outIt).priceBuy = newLine.priceBuy;
        }else{
            outIt = out.list.insert(out.list.end(), OutElement{newLine.code, newLine.priceBuy, 0., 0., 0., 0.});
        }
        if(!list.empty()){
            auto findDate = [&](const QDate &d){
                return std::find_if(list.rbegin(), list.rend(),
                                    [&](const CacheElement &line){return line.date <= d
                            && newLine.code == line.code;});
            };
            auto updateDerivation = [&](float &derivation, const QDate &date){
                if(auto onDate = findDate(date); onDate != list.rend()){
                    derivation = (outIt->priceBuy - onDate->priceBuy) / onDate->priceBuy * 100.;
                }
            };
            updateDerivation(outIt->derivation, newLine.date.addDays(-1));
            updateDerivation(outIt->derivationWeek, newLine.date.addDays(-7));
            updateDerivation(outIt->derivationMonth, newLine.date.addDays(-30));
            updateDerivation(outIt->derivationYear, newLine.date.addDays(-365));
        }
        cached = out;
    }
    list.push_back(newLine);
    list.erase(std::remove_if(list.begin(), list.end(), [&](CacheElement &line){
                   return line.date.daysTo(newLine.date) > 365
                   && newLine.code == line.code;}),
            list.end());
}
const CbrfMetalCacheDb::CacheElement * CbrfMetalCacheDb::getLine(const QDate &date, const QByteArray &code)
{
    auto it = std::find_if(list.begin(), list.end(), [&](CacheElement &line){
            return line.date.daysTo(date) >= 0
            && code == line.code;});
    if(it == list.end()){
        return nullptr;
    }else{
        return &(*it);
    }
}

CbrfMetalCacheDb::OutLine CbrfMetalCacheDb::getCached()
{
    return cached;
}
