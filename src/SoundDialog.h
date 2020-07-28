#ifndef SOUNDDIALOG_H
#define SOUNDDIALOG_H

#include <QDialog>
#include <memory>
#include <QSound>

namespace Ui {
class SoundDialog;
}

class SoundDialog : public QDialog
{
    Q_OBJECT

    const QString dir;
    std::shared_ptr<QSound> sound;

    void setupFile(const QString &filename);
public:
    explicit SoundDialog(const QString dir, QWidget *parent = nullptr);
    ~SoundDialog();

    static QString getSoundFileName(const QString dir = QString());

private slots:
    void on_selectButton_clicked();

    void on_playButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::SoundDialog *ui;
};

#endif // SOUNDDIALOG_H
