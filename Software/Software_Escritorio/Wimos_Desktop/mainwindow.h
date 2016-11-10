#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringListModel>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QPair>
#include <QDateTime>
#include <QVectorIterator>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);


    ~MainWindow();

private:
    void importTestValues();
    void getValuesSQL(const QString& mac);
    void connectBD();
    void getStationsOperative();
    void setupPlot();

    Ui::MainWindow *ui;
    QSqlDatabase *myBD;
    QTimer *timer2;
    QTimer *timer;
    QVector<QPair<double,QDateTime> > graphValue;
    QDateTime lastTime;

public slots:
    void displayValues();
    void drawPlot();

};

#endif // MAINWINDOW_H
