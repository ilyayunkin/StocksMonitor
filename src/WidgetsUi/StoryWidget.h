#ifndef STORYWIDGET_H
#define STORYWIDGET_H

#include <QDialog>

class StoryWidget : public QDialog
{
    Q_OBJECT
public:
    static void showStory();
private:
    explicit StoryWidget(QWidget *parent = nullptr);
    ~StoryWidget();
};

#endif // STORYWIDGET_H
