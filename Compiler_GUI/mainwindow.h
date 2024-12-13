

// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private slots:
//     void onBrowseButtonClicked();
//     void onScannerButtonClicked();
//     void onParserButtonClicked();

// private:
//     Ui::MainWindow *ui;
// };

// #endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Node.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include <QPointF>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBrowseButtonClicked();
    void onScannerButtonClicked();
    void onParserButtonClicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene; // Scene for rendering the tree

    // void drawTree(Node *node, QPointF position, qreal xOffset = 150); // Draw tree on the scene
    // void deleteTree(Node *node); // Clean up tree memory



    void drawTree(Node* root, QPointF startPos, qreal horizontalSpacing);
    void calculateNodePositions(Node* node, int depth, int& xPosition, std::map<Node*, QPointF>& positions, qreal horizontalSpacing);
    void drawTreeWithPrecomputedPositions(Node* node, const std::map<Node*, QPointF>& positions);
};

#endif // MAINWINDOW_H


