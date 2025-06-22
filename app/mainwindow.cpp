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
        "QMainWindow {"
        "   background: #f8fafc;"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox {"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 10px;"
        "   margin-top: 15px;"
        "   padding-top: 15px;"
        "   background: white;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 4px;"
        "   color: #39af58;"
        "   font-weight: 600;"
        "   font-size: 14px;"
        "}"
        "QPushButton {"
        "   background: white;"
        "   color: #39af58;"
        "   border: 1px solid #e2e8f0;"
        "   padding: 4px 8px;"
        "   border-radius: 6px;"
        "   font-weight: 500;"
        "   transition: all 0.2s ease;"
        "}"
        "QPushButton:hover {"
        "   background: #39af58;"
        "   color: white;"
        "   border-color: #39af58;"
        "}"
        "QLineEdit, QTextEdit, QComboBox, QDoubleSpinBox {"
        "   padding: 4px 6px;"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 6px;"
        "   background: white;"
        "   selection-background-color: #39af58;"
        "   selection-color: white;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QComboBox:focus {"
        "   border-color: #39af58;"
        "   outline: none;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 24px;"
        "}"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: #f1f5f9;"
        "   width: 10px;"
        "   margin: 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #cbd5e1;"
        "   min-height: 20px;"
        "   border-radius: 5px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   border: none;"
        "   background: none;"
        "   height: 0;"
        "}"
        "QTabWidget::pane {"
        "   border: 1px solid #e2e8f0;"
        "   border-radius: 6px;"
        "   padding: 6px;"
        "   margin-top: -1px;"
        "}"
        "QTabBar::tab {"
        "   padding: 8px 16px;"
        "   border: none;"
        "   color: #64748b;"
        "}"
        "QTabBar::tab:selected {"
        "   color: #39af58;"
        "   border-bottom: 2px solid #39af58;"
        "}"
        "QMessageBox, QInputDialog, QDialog {"
        "background: #f8fafc;"
        "font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QMessageBox QLabel {"
        "   color: #334155;"
        "}"
        "QGraphicsView {"
        "   border: 1px solid #d1d8e0;"
        "   border-radius: 8px;"
        "   background: white;"
        "}"
    );
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->findButton, &QPushButton::clicked, this, &MainWindow::onFindClicked);
    connect(ui->getButton, &QPushButton::clicked, this, &MainWindow::onGetClicked);
    connect(ui->traverseButton, &QPushButton::clicked, this, &MainWindow::onTraverseClicked);
    connect(ui->serializeButton, &QPushButton::clicked, this, &MainWindow::onSerializeClicked);
    connect(ui->deserializeButton, &QPushButton::clicked, this, &MainWindow::onDeserializeClicked);
    drawTree();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddClicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "Добавить элемент", "Введите элемент:", 0.0, -1e9, 1e9, 2, &ok);
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
    double value = QInputDialog::getDouble(this, "Удалить элемент", "Введите элемент:", 0.0, -1e9, 1e9, 2, &ok);
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
    tree = BinaryTree<double>();
    drawTree();
    ui->statusbar->showMessage("Бинарное дерево очищено", 3000);
}

void MainWindow::onFindClicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "Найти элемент", "Введите элемент:", 0.0, -1e9, 1e9, 2, &ok);
    if (!ok) return;
    bool found = tree.FindElement(value);
    QMessageBox::information(this, "Результат поиска",
                             found ? QString("Элемент (%1) найден").arg(value)
                                   : QString("Элемент (%1) не найден").arg(value));
}

void MainWindow::onGetClicked() {
    bool ok;
    double value = QInputDialog::getDouble(this, "Извлечь поддерево", "Введите элемент:", 0.0, -1e9, 1e9, 2, &ok);
    if (!ok) return;
    try {
        tree = tree.GetSubTree(value);
        drawTree();
        ui->statusbar->showMessage(QString("Поддерево с корнем: %1").arg(value), 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
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

void MainWindow::onSerializeClicked() {
    bool ok;
    QString format = QInputDialog::getText(this, "Сериализация", "Введите формат сериализации (например, {K}(L)[P]):",
                                           QLineEdit::Normal, "{K}(L)[P]", &ok);
    if (!ok || format.isEmpty()) return;
    try {
        string serialized = tree.SaveString(format.toStdString());
        QMessageBox::information(this, "Сериализация", QString("Результат сериализации:\n%1").arg(QString::fromStdString(serialized)));
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onDeserializeClicked() {
    bool ok;
    QString format = QInputDialog::getText(this, "Десериализация", "Введите формат десериализации (например, {K}(L)[P]):",
                                           QLineEdit::Normal, "{K}(L)[P]", &ok);
    if (!ok || format.isEmpty()) return;
    QString data = QInputDialog::getText(this, "Десериализация", "Введите строку для десериализации:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || data.isEmpty()) return;
    try {
        tree.ReadString(format.toStdString(), data.toStdString());
        drawTree();
        ui->statusbar->showMessage("Дерево успешно десериализовано", 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::drawNode(QGraphicsScene *scene, Node<double>* node, int x, int y, int hGap) {
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
