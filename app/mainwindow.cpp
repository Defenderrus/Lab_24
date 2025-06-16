#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    setStyleSheet(
        "QMainWindow { background: #f0f2f5; }"
        "QGroupBox { border: 1px solid #d1d8e0; border-radius: 8px; margin-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; color: #39af58; }"
        "QPushButton { background: white; color: #39af58; border: none; padding: 8px 12px; border-radius: 4px; }"
        "QPushButton:hover { color: white; background: #39af58; }"
        "QLineEdit, QComboBox { padding: 6px; border: 1px solid #d1d8e0; border-radius: 4px; }"
        "QGraphicsView { border: 1px solid #d1d8e0; border-radius: 8px; background: white; }"
    );
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->findButton, &QPushButton::clicked, this, &MainWindow::onFindClicked);
    connect(ui->traverseButton, &QPushButton::clicked, this, &MainWindow::onTraverseClicked);
    drawTree();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddClicked() {
    bool ok;
    int value = QInputDialog::getInt(this, "Добавить элемент", "Введите элемент:", 0, -1000, 1000, 1, &ok);
    if (!ok) return;
    try {
        tree.Add(value);
        drawTree();
        ui->statusbar->showMessage(QString("Добавлен элемент: %1").arg(value), 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onRemoveClicked() {
    bool ok;
    int value = QInputDialog::getInt(this, "Удалить элемент", "Введите элемент:", 0, -1000, 1000, 1, &ok);
    if (!ok) return;
    try {
        tree.Remove(value);
        drawTree();
        ui->statusbar->showMessage(QString("Удалённый элемент: %1").arg(value), 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onClearClicked() {
    tree = BinaryTree<int>();
    drawTree();
    ui->statusbar->showMessage("Бинарное дерево очищено", 3000);
}

void MainWindow::onFindClicked() {
    bool ok;
    int value = QInputDialog::getInt(this, "Найти элемент", "Введите элемент:", 0, -1000, 1000, 1, &ok);
    if (!ok) return;
    bool found = tree.FindElement(value);
    QMessageBox::information(this, "Результат поиска",
                             found ? QString("Элемент (%1) найден").arg(value)
                                   : QString("Элемент (%1) не найден").arg(value));
}

void MainWindow::onTraverseClicked() {
    QString type = ui->traverseComboBox->currentText();
    auto sequence = tree.Traversal(type.toStdString());
    QString result;
    for (int i = 0; i < sequence->GetLength(); i++) {
        result += QString::number(sequence->Get(i)) + " ";
    }
    QMessageBox::information(this, "Обход", QString("%1 обход: %2").arg(type).arg(result));
}

void MainWindow::drawNode(QGraphicsScene *scene, Node<int>* node, int x, int y, int hGap) {
    if (!node) return;
    scene->addEllipse(x-15, y-15, 30, 30, QPen(Qt::black), QBrush(QColor("#39af58")));
    QGraphicsTextItem *text = scene->addText(QString::number(node->data));
    text->setDefaultTextColor(Qt::white);
    text->setPos(x - text->boundingRect().width()/2, y - text->boundingRect().height()/2);
    if (node->left) {
        int leftX = x - hGap;
        int leftY = y + 60;
        scene->addLine(x, y+15, leftX, leftY-15, QPen(Qt::black, 1));
        drawNode(scene, node->left, leftX, leftY, hGap/2);
    }
    if (node->right) {
        int rightX = x + hGap;
        int rightY = y + 60;
        scene->addLine(x, y+15, rightX, rightY-15, QPen(Qt::black, 1));
        drawNode(scene, node->right, rightX, rightY, hGap/2);
    }
}

void MainWindow::drawTree() {
    scene->clear();
    if (tree.GetRoot()) {
        drawNode(scene, tree.GetRoot(), ui->graphicsView->width()/2, 30, 120);
    }
    ui->graphicsView->centerOn(0, 0);
}
