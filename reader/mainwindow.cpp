#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeView>
#include <iostream>
#include <thread>
#include <QMessageBox>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Reader");
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("work_db.db");
    if(!db.open()) return;

    QSqlQuery pragma(db);

    // встроенное решение от проблемы с блокировкой доступа к sqlite3
    pragma.exec("PRAGMA journal_mode=WAL;");
    pragma.exec("PRAGMA busy_timeout=5000;");

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

    connect(this, &MainWindow::ready_read,
            this, &MainWindow::read_in_stream,
            Qt::QueuedConnection);



  //  thr.detach();

    thr = std::thread(&MainWindow::asyncReciveData, this);
    thr.detach();
}

MainWindow::~MainWindow()
{
    recive_data = false;
    if(thr.joinable())
        thr.join();

    delete ui;
}

void MainWindow::create_model()
{
    model = new QSqlTableModel;
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

std::string MainWindow::getRnd_digital()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-100.0, 100.0);
    return std::to_string(dist(gen));
}

void MainWindow::read_in_stream(){
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
    db.open();
    model->select();
   // model->setQuery("SELECT id, x, y, z, Vx, Vy, Vz FROM db");

   // model->setHeaderData(0, Qt::Horizontal, "id");
   // model->setHeaderData(1, Qt::Horizontal, "x");
   // model->setHeaderData(2, Qt::Horizontal, "y");
   // model->setHeaderData(3, Qt::Horizontal, "z");
   // model->setHeaderData(4, Qt::Horizontal, "Vx");
   // model->setHeaderData(5, Qt::Horizontal, "Vy");
   // model->setHeaderData(6, Qt::Horizontal, "Vz");
}

void MainWindow::asyncReciveData()
{
    while(true) {

        if(recive_data){
            emit ready_read();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
}

// (write example)
void MainWindow::on_pushButton_clicked()
{

    QString sql = QString("VALUES (%1, %2, %3, %4, %5, %6)")
    .arg(getRnd_digital())
        .arg(getRnd_digital())
        .arg(getRnd_digital())
        .arg(getRnd_digital())
        .arg(getRnd_digital())
        .arg(getRnd_digital());
    QSqlQuery query;
    bool ok = query.exec(
        "INSERT INTO db (x, y, z, Vx, Vy, Vz) " + sql
        );
}

// метод для запуска флага на получение данных
void MainWindow::on_pushButton_2_clicked()
{
    if(!recive_data){
        recive_data = true;
        ui->pushButton_2->setText("stop recive data");
    }else{
        recive_data = false;
        ui->pushButton_2->setText("start recive data");
    }


}

