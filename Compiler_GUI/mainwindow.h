

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

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include <QPointF>
#include <vector>

// TreeNode structure
struct TreeNode {
    QString label;
    std::vector<TreeNode *> children;
};

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
    TreeNode *rootNode;    // Root of the syntax tree

    TreeNode* parseInput(const QString &input); // Method to build the syntax tree
    void drawTree(TreeNode *node, QPointF position, qreal xOffset = 150); // Draw tree on the scene
    void deleteTree(TreeNode *node); // Clean up tree memory
};

#endif // MAINWINDOW_H


