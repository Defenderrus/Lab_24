#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "../BinaryTree.hpp"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onClearClicked();
    void onFindClicked();
    void onTraverseClicked();
private:
    Ui::MainWindow *ui;
    BinaryTree<double> tree;
    QGraphicsScene *scene;

    void drawNode(QGraphicsScene *scene, Node<double>* node, int x, int y, int hGap);
    void drawTree();
};

#endif // MAINWINDOW_H
