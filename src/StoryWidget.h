#ifndef STORYWIDGET_H
#define STORYWIDGET_H

#include <QDialog>

class StoryWidget : public QDialog
{
    Q_OBJECT
    explicit StoryWidget(QWidget *parent = nullptr);
    ~StoryWidget();
public:
    static void showStory();
};

#endif // STORYWIDGET_H
