#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <iostream>
#include <thread>
#include <random>
#include <string>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void ready_read();

private slots:
    void read_in_stream();
private:
    Ui::MainWindow *ui;

  //  bool recive_data = false;


    QSqlTableModel* model;
    void create_model();


    std::atomic<bool> recive_data = false;
    std::string getRnd_digital();// для тестовой кнопки на создание
    // метод под асинхронное обновление данных
    void asyncReciveData();
    std::thread thr;
};
#endif // MAINWINDOW_H
