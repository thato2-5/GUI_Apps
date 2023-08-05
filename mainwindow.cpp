#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons to their slots
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->convertPushButton, &QPushButton::clicked, this, &MainWindow::calculate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    ui->distanceLineEdit->clear();
    ui->temperatureLineEdit->clear();
    ui->temperatureLineEdit->setFocus();
}

void MainWindow::calculate()
{
    QString temperature_string, temperature_result, distance_string, distance_result;
    float temperature, distance, result_temperature, result_distance;

    temperature_string = ui->temperatureLineEdit->text();
    distance_string = ui->distanceLineEdit->text();

    if ( temperature_string.isEmpty() || distance_string.isEmpty() )
    {
        QMessageBox::critical(this, "Error", "Please enter both tempearture and distance !");
        ui->temperatureLineEdit->clear();
        ui->distanceLineEdit->clear();
        ui->temperatureLineEdit->setFocus();
        return;
    }

    temperature = temperature_string.toFloat();
    distance = distance_string.toFloat();

    result_temperature = (temperature * 9 / 5) + 32;
    result_distance = (distance * 39.37);

    temperature_result = QString::number(result_temperature);
    distance_result = QString::number(result_distance);

    QMessageBox::information(this, "Success", "Temperature in fahrenheit: " + temperature_result + "\nDistance in inches: " + distance_result);

    ui->temperatureLineEdit->clear();
    ui->distanceLineEdit->clear();
    ui->temperatureLineEdit->setFocus();

}
