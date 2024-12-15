#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Scanner.h"
#include "parser.h"
#include "Node.h"
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
#include <QKeyEvent>

#include "TreeViewerWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseButtonClicked);
    connect(ui->scannerButton, &QPushButton::clicked, this, &MainWindow::onScannerButtonClicked);
    connect(ui->parserButton, &QPushButton::clicked, this, &MainWindow::onParserButtonClicked);
}

MainWindow::~MainWindow()
{
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
    inputText = inputText.trimmed();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Input text is empty.");
        return;
    }

    Scanner scanner(inputText.toStdString());

    string tokensList = scanner.GetTokensList();
    if (tokensList.empty()) {
        QMessageBox::warning(this, "Error", "No valid tokens found.");
        return;
    }

    // Check if the last line contains "invalid symbol"
    QString tokensListQt = QString::fromStdString(tokensList);
    // Display tokens list in the output text edit
    ui->outputTextEdit->setPlainText(tokensListQt);

    QStringList lines = tokensListQt.split("\n", Qt::SkipEmptyParts);
    if (!lines.isEmpty() && lines.last().trimmed() == "Invalid symbol") {
        QMessageBox::critical(this, "Error", "Invalid symbol detected in the input.");
        return;
    }


}


void MainWindow::onParserButtonClicked()
{
    QString inputText = ui->inputTextEdit->toPlainText();

    // Check if the input text is empty
    inputText = inputText.trimmed(); // Removes leading and trailing whitespaces
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Input text is empty.");
        return; // Don't proceed if input is empty
    }

    Parser parser(inputText.toStdString());
    Node* rootNode = parser.GetSyntaxTree();  // Use rootNode to store the tree*/


    if (rootNode == nullptr) {
        QMessageBox::warning(this, "Error", "Syntax tree could not be generated.");
        return;
    }


    // drawTree(rootNode, QPointF(0, 0), 150.0);

    // // Create a new TreeViewerWindow
    // TreeViewerWindow *treeWindow = new TreeViewerWindow(this);

    // // Set the scene for the new window
    // treeWindow->setScene(scene);


    // // Show the new window
    // treeWindow->show();

    scene->clear();
    // drawTree(rootNode, QPointF(400, 0));
    drawTree(rootNode, QPointF(0, 0), 150.0);
     // Set a larger, more readable font

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(1400, 800);
    view->setWindowTitle("Syntax Tree Viewer");
    view->setFont(QFont("Arial", 30));


    // Dynamically adjust the scale of the view based on the tree's bounding box
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    view->scale(2, 2);

    view->show();

 }


void MainWindow::calculateNodePositions(Node* node, int depth, int& xPosition, std::map<Node*, QPointF>& positions, qreal horizontalSpacing) {
    if (!node) return;

    // Left child: Traverse first (DFS)
    calculateNodePositions(node->leftChild, depth + 1, xPosition, positions, horizontalSpacing);

    // Assign position for the current node
    positions[node] = QPointF(xPosition * horizontalSpacing, depth * 100.0);
    xPosition++; // Move to the next horizontal position

    // Right child: Traverse next
    calculateNodePositions(node->rightChild, depth + 1, xPosition, positions, horizontalSpacing);

    // Sibling: Traverse last
    calculateNodePositions(node->sibling, depth, xPosition, positions, horizontalSpacing);

}

void MainWindow::drawTreeWithPrecomputedPositions(Node* node, const std::map<Node*, QPointF>& positions) {
    if (!node) return;

    QPointF currentPos = positions.at(node);

    // Calculate dynamic size based on text
    QString text = QString::fromStdString(node->value);
    QFont largeFont("Arial", 14);  // You can adjust the font size here
    QFontMetrics metrics(largeFont);
    int textWidth = metrics.horizontalAdvance(text) + 20;  // Add padding
    int textHeight = metrics.height() + 10;

    // Adjust ellipse size based on text
    qreal nodeWidth = qMax(120.0, static_cast<qreal>(textWidth));  // Minimum width
    qreal nodeHeight = qMax(60.0, static_cast<qreal>(textHeight)); // Minimum height

    // Conditional check to determine whether to draw a rectangle or ellipse
    if (node->type == "stmt") {
        // Draw a rectangle if node->type == "stmt"
        QGraphicsRectItem* nodeItem = scene->addRect(currentPos.x(), currentPos.y(), nodeWidth, nodeHeight, QPen(Qt::white));
        QGraphicsTextItem* textItem = scene->addText(text);
        textItem->setPos(currentPos.x() + (nodeWidth - textWidth) / 2, currentPos.y() + 0.5*(nodeHeight - textHeight) / 2);
        textItem->setFont(largeFont);  // Set the larger font
    } else {
        // Draw an ellipse for other types
        QGraphicsEllipseItem* nodeItem = scene->addEllipse(currentPos.x(), currentPos.y(), nodeWidth, nodeHeight, QPen(Qt::white));
        QGraphicsTextItem* textItem = scene->addText(text);
        textItem->setPos(currentPos.x() + (nodeWidth - textWidth) / 2, currentPos.y() + 0.5*(nodeHeight - textHeight) / 2);
        textItem->setFont(largeFont);  // Set the larger font
    }


    // Draw line to left child
    if (node->leftChild) {
        QPointF childPos = positions.at(node->leftChild);
        scene->addLine(currentPos.x() + nodeWidth / 2, currentPos.y() + nodeHeight,
                       childPos.x() + nodeWidth / 2, childPos.y(), QPen(Qt::white));
        drawTreeWithPrecomputedPositions(node->leftChild, positions);
    }

    // Draw line to sibling
    if (node->sibling) {
        QPointF siblingPos = positions.at(node->sibling);
        scene->addLine(currentPos.x() + nodeWidth, currentPos.y() + nodeHeight / 2,
                       siblingPos.x(), siblingPos.y() + nodeHeight / 2, QPen(Qt::white));
        drawTreeWithPrecomputedPositions(node->sibling, positions);
    }

    // Draw line to right child
    if (node->rightChild) {
        QPointF childPos = positions.at(node->rightChild);
        scene->addLine(currentPos.x() + nodeWidth / 2, currentPos.y() + nodeHeight,
                       childPos.x() + nodeWidth / 2, childPos.y(), QPen(Qt::white));
        drawTreeWithPrecomputedPositions(node->rightChild, positions);
    }
}

void MainWindow::drawTree(Node* root, QPointF startPos, qreal horizontalSpacing) {
    if (!root) return;

    // Step 1: Pre-compute positions
    std::map<Node*, QPointF> positions;
    int xPosition = 0;
    calculateNodePositions(root, 0, xPosition, positions, horizontalSpacing);

    // Step 2: Draw tree based on pre-computed positions
    drawTreeWithPrecomputedPositions(root, positions);
}




void MainWindow::on_CompileButton_clicked()
{
    // Placeholder for scanner functionality
    QString inputText = ui->inputTextEdit->toPlainText();
    inputText = inputText.trimmed();
    if (inputText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Input text is empty.");
        return;
    }

    Scanner scanner(inputText.toStdString());

    string tokensList = scanner.GetTokensList();
    if (tokensList.empty()) {
        QMessageBox::warning(this, "Error", "No valid tokens found.");
        return;
    }

    // Check if the last line contains "invalid symbol"
    QString tokensListQt = QString::fromStdString(tokensList);
    // Display tokens list in the output text edit
    ui->outputTextEdit->setPlainText(tokensListQt);

    QStringList lines = tokensListQt.split("\n", Qt::SkipEmptyParts);
    if (!lines.isEmpty() && lines.last().trimmed() == "Invalid symbol") {
        QMessageBox::critical(this, "Error", "Invalid symbol detected in the input.");
        return;
    }


    Parser parser(tokensList);
    Node* rootNode = parser.GetSyntaxTree();  // Use rootNode to store the tree*/


    if (rootNode == nullptr) {
        QMessageBox::warning(this, "Error", "Syntax tree could not be generated.");
        return;
    }


    scene->clear();
    // drawTree(rootNode, QPointF(400, 0));
    drawTree(rootNode, QPointF(0, 0), 150.0);
        // Set a larger, more readable font

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(1200, 800);
    view->setWindowTitle("Syntax Tree Viewer");
    view->setFont(QFont("Arial", 30));


    // Dynamically adjust the scale of the view based on the tree's bounding box
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    view->scale(1.5, 1.5);

    view->show();


    // drawTree(rootNode, QPointF(0, 0), 150.0);

    // // Create a new TreeViewerWindow
    // TreeViewerWindow *treeWindow = new TreeViewerWindow(this);

    // // Set the scene for the new window
    // treeWindow->setScene(scene);


    // // Show the new window
    // treeWindow->show();


}

