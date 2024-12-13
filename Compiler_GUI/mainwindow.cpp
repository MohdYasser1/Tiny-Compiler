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


}

// void MainWindow::drawTree(Node* node, QPointF position, qreal xOffset)
// {
//     if (!node) return;

//     // Draw the current node
//     QGraphicsItem* nodeItem;
//     if (node->type == "stmt") {
//         nodeItem = scene->addRect(position.x(), position.y(), 100, 40);
//     } else {  // type == "exp"
//         nodeItem = scene->addEllipse(position.x(), position.y(), 80, 40);
//     }
//     scene->addText(node->value)->setPos(position.x() + 10, position.y() + 5);

//     // Draw the leftChild
//     if (node->leftChild) {
//         QPointF childPos = position + QPointF(-xOffset, 100);
//         scene->addLine(position.x() + 50, position.y() + 40, childPos.x() + 50, childPos.y());
//         drawTree(node->leftChild, childPos, xOffset / 1.5);
//     }

//     // Draw the rightChild
//     if (node->rightChild) {
//         QPointF childPos = position + QPointF(xOffset, 100);
//         scene->addLine(position.x() + 50, position.y() + 40, childPos.x() + 50, childPos.y());
//         drawTree(node->rightChild, childPos, xOffset / 1.5);
//     }

//     // Draw siblings
//     if (node->sibling) {
//         QPointF siblingPos = position + QPointF(xOffset, 0);
//         scene->addLine(position.x() + 100, position.y() + 20, siblingPos.x(), siblingPos.y() + 20);
//         drawTree(node->sibling, siblingPos, xOffset);
//     }
// }

