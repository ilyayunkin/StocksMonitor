#include "StatisticsWidget.h"
#include "ui_StatisticsWidget.h"
#include "StatisticsModel.h"

#include <algorithm>

#include <QDebug>
#include <QInputDialog>

void StatisticsWidget::updateStocksList(const QString &pluginSelected)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << pluginSelected;
    ui->stocksComboBox->clear();
    auto stocks = this->application.getStockIdList(pluginSelected);
    if(!stocks.empty())
    {
        auto comparator = [](auto const &lhs, auto const &rhs)
        {
            return lhs.name < rhs.name;
        };
        std::sort(stocks.begin(), stocks.end(), comparator);

        const auto addId = [this](const auto &id)
        {
            ui->stocksComboBox->addItem(
                        QString("%1(%2)")
                        .arg(id.name)
                        .arg(id.ticker.data())
                        , id.ticker.data());
        };

        std::for_each(stocks.begin(), stocks.end(), addId);
    }else
    {
        ui->stocksComboBox->addItem(tr("No items - try later"));
    }
}

StatisticsWidget::StatisticsWidget(Application &application, QWidget *parent)
    : QWidget(parent)
    , application(application)
    , configController(application.getStatisticsController())
    , model(new StatisticsModel(configController.getStatisticsConfig(), this))
    , ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
    ui->pluginsComboBox->addItems(application.getPluginsList());
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    application.setStatisticsConfigView(this);
    auto updateEnables = [this](const QModelIndex &index)
    {
        auto type = model->getType(index);
        switch (type) {
        case StatisticsModelElementType::ROOT:
            ui->addButton->setEnabled(true);
            ui->removeButton->setEnabled(false);
            ui->pluginsComboBox->setEnabled(false);
            ui->stocksComboBox->setEnabled(false);
            break;
        case StatisticsModelElementType::CATEGORY:
            ui->addButton->setEnabled(true);
            ui->removeButton->setEnabled(true);
            ui->pluginsComboBox->setEnabled(false);
            ui->stocksComboBox->setEnabled(false);
            break;
        case StatisticsModelElementType::GROUP:
            ui->addButton->setEnabled(true);
            ui->removeButton->setEnabled(true);
            ui->pluginsComboBox->setEnabled(true);
            ui->stocksComboBox->setEnabled(true);
            break;
        case StatisticsModelElementType::STOCK:
            ui->addButton->setEnabled(false);
            ui->removeButton->setEnabled(true);
            ui->pluginsComboBox->setEnabled(true);
            ui->stocksComboBox->setEnabled(true);
            break;
        default:
        case StatisticsModelElementType::NONE:
            ui->addButton->setEnabled(false);
            ui->removeButton->setEnabled(false);
            ui->pluginsComboBox->setEnabled(false);
            ui->stocksComboBox->setEnabled(false);
            break;
        }
    };
    connect(ui->treeView, &QTreeView::clicked, updateEnables);

    // Here should be QComboBox::textActivated, which is introduced in the Qt 5.14
    // But Travis CI uses 15.12 currently.
    connect(ui->pluginsComboBox, QOverload<const QString &>::of(&QComboBox::activated),
            this, &StatisticsWidget::updateStocksList);

    connect(ui->processButton, &QPushButton::clicked,
            [&application](){application.getStatisticsController().processStatistics();});

    auto add = [this]
    {
        auto index = ui->treeView->selectionModel()->currentIndex();
        auto type = model->getType(index);
        auto configElement = model->getTreeElement(index);
        switch (type) {
        case StatisticsModelElementType::ROOT:
        {
            const auto categoryName = QInputDialog::getText(this, tr("Input category name"), tr("Input category name"));
            if(!categoryName.isEmpty())
            {
                StatisticsConfigCategoryIndex catIndex(categoryName, static_cast<void *>(configElement));
                configController.addCategory(catIndex);
            }
        }
            break;
        case StatisticsModelElementType::CATEGORY:
        {
            const auto groupName = QInputDialog::getText(this, tr("Input group name"), tr("Input group name"));
            if(!groupName.isEmpty())
            {
                StatisticsConfigGroupIndex groupIndex(configElement->getName()
                                                      , groupName
                                                      , static_cast<void *>(configElement));
                configController.addGroup(groupIndex);
            }
        }
            break;
        case StatisticsModelElementType::GROUP:
        {
            const auto ticker = ui->stocksComboBox->itemData(ui->stocksComboBox->currentIndex()).toByteArray();
            if(!ticker.isEmpty())
            {
                const auto name = ui->stocksComboBox->currentText();
                StatisticsConfigItemIndex itemIndex(configElement->getParent()->getName()
                                                    , configElement->getName()
                                                    , StockId{ticker.data(), name}
                                                    , static_cast<void *>(configElement));
                configController.addItem(itemIndex);
            }
            break;
        }
        default:
        case StatisticsModelElementType::STOCK:
        case StatisticsModelElementType::NONE:
            break;
        }

    };
    connect(ui->addButton, &QPushButton::clicked, add);
    auto remove = [this]
    {
        auto index = ui->treeView->selectionModel()->currentIndex();
        auto type = model->getType(index);
        auto configElement = model->getTreeElement(index);
        auto parentElement = configElement->getParent();
        switch (type) {
        case StatisticsModelElementType::CATEGORY:
        {
            StatisticsConfigCategoryIndex catIndex(configElement->getName()
                                                   , static_cast<void *>(configElement));
            configController.removeCategory(catIndex);
        }
            break;
        case StatisticsModelElementType::GROUP:
        {
            StatisticsConfigGroupIndex groupIndex(parentElement->getName()
                                                  , configElement->getName()
                                                  , static_cast<void *>(configElement));
            configController.removeGroup(groupIndex);
        }
            break;
        case StatisticsModelElementType::STOCK:
        {
            const auto ticker = configElement->getTicker();
            assert(!ticker.isEmpty());

            const auto name = configElement->getName();
            StatisticsConfigItemIndex itemIndex(configElement->getParent()->getParent()->getName()
                                                , configElement->getParent()->getName()
                                                , StockId{ticker.data(), name}
                                                , static_cast<void *>(configElement));
            configController.removeItem(itemIndex);
            break;
        }
        default:
        case StatisticsModelElementType::ROOT:
        case StatisticsModelElementType::NONE:
            break;
        }
    };
    connect(ui->removeButton, &QPushButton::clicked, remove);
    updateStocksList(ui->stocksComboBox->currentText());
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::addItem(const StatisticsConfigItemIndex &index)
{
    auto parentElement = static_cast<StatisticsTreeElement *>(index.internalPointer);
    model->addItem(parentElement, index.stock.name, index.stock.ticker.data());
}

void StatisticsWidget::addGroup(const StatisticsConfigGroupIndex &index)
{
    auto parentElement = static_cast<StatisticsTreeElement *>(index.internalPointer);
    model->addGroup(parentElement, index.group);
}

void StatisticsWidget::addCategory(const StatisticsConfigCategoryIndex &index)
{
    model->addCategory(index.category);
}

void StatisticsWidget::removeItem(const StatisticsConfigItemIndex &index)
{
    auto element = static_cast<StatisticsTreeElement *>(index.internalPointer);
    model->remove(element);
}

void StatisticsWidget::removeGroup(const StatisticsConfigGroupIndex &index)
{
    auto element = static_cast<StatisticsTreeElement *>(index.internalPointer);
    model->remove(element);
}

void StatisticsWidget::removeCategory(const StatisticsConfigCategoryIndex &index)
{
    auto element = static_cast<StatisticsTreeElement *>(index.internalPointer);
    model->remove(element);
}
