#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    graphValue =  QVector<QPair<double,QDateTime> >(1000, QPair<double,QDateTime>(0,QDateTime::currentDateTime()));
    for(int i=0; i<graphValue.length(); i++){
       graphValue[i].second = graphValue[i].second.addMSecs(-i*10);
    }
    */

    timer = new QTimer(this);
    timer->start(5000);

    setupPlot();

    connectBD();
    getStationsOperative();

    connect(timer, SIGNAL(timeout()), this, SLOT(displayValues()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBD(){
    // NECESITAMOS IMPORTAR libmysql.dll DE 32BITS EN LA CARPETA DEL EXPORTAR PROYECTO.
    myBD = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));

    myBD->setDatabaseName("asadb");
    myBD->setHostName("91.121.168.126");
    myBD->setPort(3306);
    myBD->setUserName("asadbuser");
    myBD->setPassword("tmHcw5PVAPi@5");
    if(!myBD->open()){
        QMessageBox::critical(this,"Error",myBD->lastError().text());
        return;
    }
}


void MainWindow::getValuesSQL(const QString& mac){
    if(mac != ""){
        QSqlQuery qry(*myBD);
        QDateTime firstTime,lastTimeAux;
        qint64 delayTime = 0;
        if(qry.exec("SELECT Value, DateTimeBD FROM stationvalues WHERE DateTimeBD < DATE_ADD(NOW(),INTERVAL -10 SECOND) AND StationsMAC = \""+mac+"\" ORDER BY DateTimeBD, ID;")){
            if(qry.last())
                lastTimeAux = QDateTime::fromString(qry.value(1).toString(),"yyyy-MM-ddThh:mm:ss");

            if(qry.first()){
                firstTime = QDateTime::fromString(qry.value(1).toString(),"yyyy-MM-ddThh:mm:ss");
                delayTime += ( lastTime.msecsTo(firstTime) + firstTime.msecsTo(lastTimeAux) ) / qry.size();
                lastTime = lastTimeAux;
                do{
                    firstTime = firstTime.addMSecs(delayTime);
                    graphValue.append(QPair<double,QDateTime>(qry.value(0).toDouble(),firstTime));
                    if(graphValue.length() > 1000)
                        graphValue.erase(graphValue.begin());
                }while(qry.next());
            }
        }else{
            QMessageBox::critical(this,"Error","Error1");
        }
    }
}

void MainWindow::getStationsOperative(){

    QSqlQuery qry(*myBD);
    QStringList resp;

    if(qry.exec("SELECT StationsMAC FROM stationvalues WHERE TIME_TO_SEC(DateTimeBD) < ( TIME_TO_SEC(NOW()) - 10 ) GROUP BY StationsMAC ORDER BY StationsMAC;"))
        while(qry.next()){
            resp << qry.value(0).toString();
        }
    ui->comboBox->addItem("");
    ui->comboBox->addItems(resp);
}

void MainWindow::drawPlot(){
    if(graphValue.length() > 0){
        // calculate two new data points:
        double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        static double lastPointKey = 0;
        if (key-lastPointKey > 0.01) // at most add point every 10 ms
        {
          double value0 = graphValue.first().first;
          graphValue.erase(graphValue.begin());

          double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
          // add data to lines:
          ui->customPlot->graph(0)->addData(key, value0);
          ui->customPlot->graph(1)->addData(key, value1);
          // set data of dots:
          ui->customPlot->graph(2)->clearData();
          ui->customPlot->graph(2)->addData(key, value0);
          ui->customPlot->graph(3)->clearData();
          ui->customPlot->graph(3)->addData(key, value1);
          // remove data of lines that's outside visible range:
          ui->customPlot->graph(0)->removeDataBefore(key-8);
          ui->customPlot->graph(1)->removeDataBefore(key-8);
          // rescale value (vertical) axis to fit the current data:
          ui->customPlot->graph(0)->rescaleValueAxis();
          ui->customPlot->graph(1)->rescaleValueAxis(true);
          lastPointKey = key;
        }
        // make key axis range scroll with the data (at a constant range size of 8):
        ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
        ui->customPlot->replot();

        // calculate frames per second:
        static double lastFpsKey;
        static int frameCount;
        ++frameCount;
        if (key-lastFpsKey > 2) // average fps over 2 seconds
        {
          ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
                , 0);
          lastFpsKey = key;
          frameCount = 0;
        }
    }
}

void MainWindow::displayValues(){
    if(ui->comboBox->currentText() != ""){
        getValuesSQL(ui->comboBox->currentText());
    }
}

void MainWindow::importTestValues(){
    QSqlQuery qry(*myBD);
    QString MAC("123"),Substation("1"),latitude("12W"),longitude("10E");
    QString command;


    for(int i=0; i<1000; i++){
        command = ("INSERT INTO stationvalues (StationsMAC,SubStation,Latitude,Longitude,DateTimeST,Value) VALUES(\""+MAC+"\",\""+Substation+"\",\""+latitude+"\",\""+longitude+"\",\"2015-10-31 15:25:04\",\""+QString::number(qrand() % ((60 + 1) +60) - 60)+"\");");
        qry.exec(command);
    }


}

void MainWindow::setupPlot(){
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));

    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect( timer, SIGNAL(timeout()), this, SLOT(drawPlot()));
    timer->start(0); // Interval 0 means to refresh as fast as possible
}
