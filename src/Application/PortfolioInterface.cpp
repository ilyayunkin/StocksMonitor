#include "PortfolioInterface.h"
#include "Entities/Entities.h"
#include "Rules/Subscriptions.h"
#include "Rules/EditPortfolioInteractor.h"

PortfolioInterface::PortfolioInterface(
        const Entities &entities,
        Subscriptions &subscriptions,
        EditPortfolioInteractor &editPortfolioInteractor)
    : entities(entities)
    , subscriptions(subscriptions)
    , editPortfolioInteractor(editPortfolioInteractor)
{
}

size_t PortfolioInterface::size() const
{
    return entities.getPortfolioSize();
}

PortfolioEntry PortfolioInterface::getPortfolioEntry(const size_t i) const
{
    return entities.getPortfolioEntry(i);
}

Stock PortfolioInterface::getStock(const size_t i) const
{
    return entities.getStockForPortfolioEntry(i);
}

void PortfolioInterface::deletePortfolioEntry(size_t row)
{
    editPortfolioInteractor.deletePortfolioEntry(row);
}

bool PortfolioInterface::setPortfolioEntryQuantity(size_t row, int quantity)
{
    return editPortfolioInteractor.setPortfolioEntryQuantity(row, quantity);
}

bool PortfolioInterface::setPortfolioEntryReferencePrice(size_t row, float referencePrice)
{
    return editPortfolioInteractor.setPortfolioEntryReferencePrice(row, referencePrice);
}

void PortfolioInterface::subscribeForChanges(AbstractStocksView *view)
{
    subscriptions.subscribeForPortfolioChanges(view);
}
