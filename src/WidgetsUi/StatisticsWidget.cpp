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
    , ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);
    ui->pluginsComboBox->addItems(application.getPluginsList());
    auto model = new StatisticsModel(application.getStatisticsController(), this);
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    auto updateEnables = [model, this](const QModelIndex &index)
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

    auto add = [model, this]
    {
        auto index = ui->treeView->selectionModel()->currentIndex();
        auto type = model->getType(index);
        switch (type) {
        case StatisticsModelElementType::ROOT:
        {
            const auto categoryName = QInputDialog::getText(this, tr("Input category name"), tr("Input category name"));
            if(!categoryName.isEmpty())
            {
                model->addCategory(index, categoryName);
            }
        }
            break;
        case StatisticsModelElementType::CATEGORY:
        {
            const auto groupName = QInputDialog::getText(this, tr("Input group name"), tr("Input group name"));
            if(!groupName.isEmpty())
            {
                model->addGroup(index, groupName);
            }
        }
            break;
        case StatisticsModelElementType::GROUP:
        {
            const auto ticker = ui->stocksComboBox->itemData(ui->stocksComboBox->currentIndex()).toByteArray();
            if(!ticker.isEmpty())
            {
                const auto name = ui->stocksComboBox->currentText();
                model->addItem(index, name, ticker);
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
    auto remove = [model, this]
    {
        auto index = ui->treeView->selectionModel()->currentIndex();
        model->remove(index);

    };
    connect(ui->removeButton, &QPushButton::clicked, remove);
    updateStocksList(ui->stocksComboBox->currentText());
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}
