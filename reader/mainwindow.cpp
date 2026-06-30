#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Reader");


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("work_db.db");
    if(!db.open()) return;

    QSqlQuery query;
    bool ok = query.exec("CREATE TABLE IF NOT EXISTS db("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "x REAL, "
                         "y REAL, "
                         "z REAL, "
                         "Vx REAL, "
                         "Vy REAL, "
                         "Vz REAL)"
                         );
    if(!ok) return;

    create_model();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_model()
{
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("db");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("id"));
    model->setHeaderData(1, Qt::Horizontal, tr("x"));
    model->setHeaderData(2, Qt::Horizontal, tr("y"));
    model->setHeaderData(3, Qt::Horizontal, tr("z"));
    model->setHeaderData(4, Qt::Horizontal, tr("Vx"));
    model->setHeaderData(5, Qt::Horizontal, tr("Vy"));
    model->setHeaderData(6, Qt::Horizontal, tr("Vz"));

    ui->treeView->setModel(model);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->show();
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->setSortingEnabled(true);

    model->setSort(0, Qt::AscendingOrder);
    model->select();


}
