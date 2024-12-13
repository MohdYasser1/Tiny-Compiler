// TreeViewerWindow.h
#ifndef TREEVIEWERWINDOW_H
#define TREEVIEWERWINDOW_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>

class TreeViewerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TreeViewerWindow(QWidget *parent = nullptr);
    void setScene(QGraphicsScene* scene);
    //void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // TREEVIEWERWINDOW_H
