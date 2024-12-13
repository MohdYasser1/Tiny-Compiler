#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Scanner.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include <QTextStream>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , rootNode(nullptr)
{
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseButtonClicked);
    connect(ui->scannerButton, &QPushButton::clicked, this, &MainWindow::onScannerButtonClicked);
    connect(ui->parserButton, &QPushButton::clicked, this, &MainWindow::onParserButtonClicked);
}

MainWindow::~MainWindow()
{
    deleteTree(rootNode);
    delete scene;
    delete ui;
}


void MainWindow::onBrowseButtonClicked()
{
    // Open a file dialog to choose the file
    QString filePath = QFileDialog::getOpenFileName(this, "Select a File", QString(), "Text Files (*.txt);;All Files (*)");

    // Check if the file was selected
    if (!filePath.isEmpty()) {
        qDebug() << "File selected:" << filePath;

        QFile file(filePath);
        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString fileContent = in.readAll();
                ui->inputTextEdit->setPlainText(fileContent);  // Set file content to inputTextEdit
            } else {
                QMessageBox::warning(this, "Error", "Cannot open the file.");
            }
        } else {
            QMessageBox::warning(this, "Error", "File does not exist.");
        }
    } else {
        QMessageBox::information(this, "File Selection", "No file was selected.");
    }
}

void MainWindow::onScannerButtonClicked()
{
    // Placeholder for scanner functionality
    QString inputText = ui->inputTextEdit->toPlainText();

    // Create a Scanner object with the input text
    Scanner scanner(inputText.toStdString());

    // Get the list of tokens from the Scanner
    string tokensList = scanner.GetTokensList();

    // Display the tokens list in the outputTextEdit
    ui->outputTextEdit->setPlainText(QString::fromStdString(tokensList));

}

void MainWindow::onParserButtonClicked()
{
    QString inputText = ui->inputTextEdit->toPlainText();

    // Delete old tree
    deleteTree(rootNode);

    // Build new tree
    rootNode = parseInput(inputText);

    // Clear scene and draw new tree
    scene->clear();
    drawTree(rootNode, QPointF(400, 0));

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(1200, 800);
    view->setWindowTitle("Syntax Tree Viewer");
    view->show();
}

TreeNode* MainWindow::parseInput(const QString &input)
{
    // Example tree for demonstration (replace this with actual parsing logic)
    TreeNode *root = new TreeNode{"program"};
    TreeNode *readNode = new TreeNode{"read (x)"};
    TreeNode *ifNode = new TreeNode{"if"};
    TreeNode *writeNode = new TreeNode{"write"};
    root->children = {readNode, ifNode, writeNode};

    TreeNode *opNode = new TreeNode{"op (<)"};
    TreeNode *assignNode = new TreeNode{"assign (fact)"};
    TreeNode *repeatNode = new TreeNode{"repeat"};
    ifNode->children = {opNode, assignNode, repeatNode};

    opNode->children = {new TreeNode{"const (0)"}, new TreeNode{"id (x)"}};
    assignNode->children = {new TreeNode{"const (1)"}};

    return root;
}

void MainWindow::drawTree(TreeNode *node, QPointF position, qreal xOffset)
{
    if (!node) return;

    // Draw the node (rectangular for statements, oval for expressions)
    QGraphicsItem *nodeItem;
    if (node->label.contains("read") || node->label.contains("if") || node->label.contains("assign")) {
        nodeItem = scene->addRect(position.x(), position.y(), 100, 40);
    } else {
        nodeItem = scene->addEllipse(position.x(), position.y(), 80, 40);
    }
    scene->addText(node->label)->setPos(position.x() + 10, position.y() + 5);

    // Draw child nodes
    QPointF childPos = position + QPointF(-xOffset * (node->children.size() - 1) / 2, 100);
    for (TreeNode *child : node->children) {
        scene->addLine(position.x() + 50, position.y() + 40, childPos.x() + 40, childPos.y());
        drawTree(child, childPos, xOffset / 1.5);
        childPos += QPointF(xOffset, 0);
    }
}

void MainWindow::deleteTree(TreeNode *node)
{
    if (!node) return;

    for (TreeNode *child : node->children) {
        deleteTree(child);
    }
    delete node;
}

