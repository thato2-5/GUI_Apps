/*
* File: mainwindow.cpp
* Author: Thato Monyamane
* Description: This is the first version of my calculator gui app that performs basic arithmatic operations using two arguments.
* Date: 01/08/2023
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

QString argument, argument_1;   // Variables to hold arguments
bool divSet = false, mulSet = false, minSet = false, addSet = false, modSet = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    argument_1.clear();
    argument_2.clear();
    currentOperator = '\0';
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_clearButton_clicked()
{
    argument_1.clear();
    argument_2.clear();
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}


void MainWindow::on_digitButton_clicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString digit = button->text();
        if (!(divSet || mulSet || minSet || addSet))
        {
            argument_1 += digit;
            ui->lineEdit->setText(argument_1);
            ui->lineEdit->setFocus();
        }
        else
        {
            argument_2 += digit;
            ui->lineEdit->setText(argument_1);
            ui->lineEdit->setFocus();
        }
    }
}

void MainWindow::on_undoPushButton_clicked()
{
    if (!(divSet || mulSet || minSet || addSet))
    {
        ui->lineEdit->backspace();
        argument_1 = ui->lineEdit->text();
        ui->lineEdit->setFocus();
    }
    else
    {
        ui->lineEdit->backspace();
        argument_2 = ui->lineEdit->text();
        ui->lineEdit->setFocus();
    }
}

void MainWindow::on_operatorButton_clicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        char op = button->text().at(0).toLatin1();
        if (op == '/')
            divSet = true;
        else if (op == 'x')
            mulSet = true;
        else if (op == '-')
            minSet = true;
        else if (op == '+')
            addSet = true;

        currentOperator = op;
        ui->lineEdit->setFocus();
    }
}

void MainWindow::on_dotPushButton_clicked()
{
    if (!(divSet || mulSet || minSet || addSet))
    {
        if (!argument_1.contains('.'))
        {
            argument_1 += '.';
            ui->lineEdit->setText(argument_1);
            ui->lineEdit->setFocus();
        }
    }
    else
    {
        if (!argument_2.contains('.'))
        {
            argument_2 += '.';
            ui->lineEdit->setText(argument_1);
            ui->lineEdit->setFocus();
        }
    }
}

void MainWindow::on_equalPushButton_clicked()
{
    calculateResult();
}

void MainWindow::calculateResult()
{
    if (divSet || mulSet || minSet || addSet)
    {
        QString answer;
        float ans, arg_1, arg_2;
        arg_1 = argument_1.toFloat();
        arg_2 = argument_2.toFloat();

        if (divSet)
        {
            if (arg_2 != 0)
            {
                ans = arg_1 / arg_2;
                answer = QString::number(ans);
                ui->lineEdit->setText(answer);
                ui->lineEdit->setFocus();
            }
            else
            {
                QMessageBox::critical(this, "Error", "Error: Division by zero");
            }
        }
        else if (mulSet)
        {
            ans = arg_1 * arg_2;
            answer = QString::number(ans);
            ui->lineEdit->setText(answer);
            ui->lineEdit->setFocus();
        }
        else if (addSet)
        {
            ans = arg_1 + arg_2;
            answer = QString::number(ans);
            ui->lineEdit->setText(answer);
            ui->lineEdit->setFocus();
        }

        argument_1.clear();
        argument_2.clear();
        divSet = false;
        mulSet = false;
        minSet = false;
        addSet = false;
    }
}

