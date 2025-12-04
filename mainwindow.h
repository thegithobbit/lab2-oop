#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimers);

    // Якщо хочеш, можна стартово очистити таблицю
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Name", "Hours", "Minutes", "Seconds", "Progress"});
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---- Обробники кнопок ----
void MainWindow::on_btnStart_clicked()
{
    timer->start(1000);  // таймер срабатывает каждую секунду
}

void MainWindow::on_btnStop_clicked()
{
    timer->stop();
}

void MainWindow::on_btnAdd_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ui->leTimerName->text()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(ui->sbHours->value())));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(ui->sbMinutes->value())));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(ui->sbSeconds->value())));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem("0%")); // прогрес поки 0%
}

void MainWindow::on_btnRemove_clicked()
{
    auto selected = ui->tableWidget->currentRow();
    if(selected >= 0)
        ui->tableWidget->removeRow(selected);
}

void MainWindow::on_btnClear_clicked()
{
    ui->tableWidget->setRowCount(0);
}

// ---- Оновлення таймерів ----
void MainWindow::updateTimers()
{
    for(int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        int h = ui->tableWidget->item(row, 1)->text().toInt();
        int m = ui->tableWidget->item(row, 2)->text().toInt();
        int s = ui->tableWidget->item(row, 3)->text().toInt();

        if(s > 0) s--;
        else {
            if(m > 0) { m--; s = 59; }
            else if(h > 0) { h--; m = 59; s = 59; }
            else continue; // таймер вже 0
        }

        ui->tableWidget->item(row, 1)->setText(QString::number(h));
        ui->tableWidget->item(row, 2)->setText(QString::number(m));
        ui->tableWidget->item(row, 3)->setText(QString::number(s));

        // Прогрес
        int totalSeconds = h*3600 + m*60 + s;
        int progress = 100 - (totalSeconds * 100 / ((ui->sbHours->value()*3600 + ui->sbMinutes->value()*60 + ui->sbSeconds->value()) + 1));
        ui->tableWidget->item(row, 4)->setText(QString::number(progress) + "%");
    }
}
