#ifndef POCKETWIDGET_H
#define POCKETWIDGET_H

#include <QWidget>
#include <PocketModel.h>

namespace Ui {
class PocketWidget;
}

class PocketWidget : public QWidget
{
    Q_OBJECT
    PocketModel &model;
public:
    explicit PocketWidget(PocketModel *model, QWidget *parent = nullptr);
    ~PocketWidget();

private slots:
    void on_clipBoardButton_clicked();
    void on_convertButton_clicked();

private:
    Ui::PocketWidget *ui;
};

#endif // POCKETWIDGET_H