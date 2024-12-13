// TreeViewerWindow.cpp
#include "TreeViewerWindow.h"

TreeViewerWindow::TreeViewerWindow(QWidget *parent)
    : QDialog(parent), view(new QGraphicsView(this)), scene(new QGraphicsScene(this))
{
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(1200, 800);
    view->setWindowTitle("Syntax Tree Viewer");
    view->setFont(QFont("Arial", 30));

    // Set the scene for the view
    view->setScene(scene);
}

void TreeViewerWindow::setScene(QGraphicsScene* newScene)
{
    scene->clear();
    scene = newScene;
    view->setScene(scene);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    view->scale(10, 10);
}
