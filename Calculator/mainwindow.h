#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_digitButton_clicked();

    void on_operatorButton_clicked();

    void on_clearButton_clicked();

    void on_undoPushButton_clicked();

    void on_equalPushButton_clicked();

    void on_dotPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString argument_1, argument_2;
    char currentOperator;

    void calculateResult();
};
#endif // MAINWINDOW_H
