#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QString>
#include <iostream>
#include <sqlite3.h>
#include <boost/format.hpp>

void MainWindow::handleFileButton(){
  QString img_path = "/home/mrk0/gelistirme/yaz1lab1/";


  QString img_file_name = img_path + ui->filePathLineEdit->text();
  //ui->file_path_button->setText(img_file_name);

  QPixmap pm(img_file_name);
  ui->inpImageLabel->setPixmap(pm);
  ui->inpImageLabel->setScaledContents(true);
}

static std::vector<std::vector<std::string>> query(sqlite3 *DB,int colCount,char* sql){
    // buralara try catchler yakısır
    sqlite3_stmt* stmt;
    if (sqlite3_open("/home/mrk0/gelistirme/yaz1lab1/mock.db", &DB) == SQLITE_OK) {
        sqlite3_prepare(DB,sql, -1, &stmt, NULL); //preapre bytecode
    } else {
        std::cout << "failed to open db" << std::endl;
    }
    std::vector<std::vector<std::string>> rows;
    int rowInd = 0;
    //int colCount = 3;
    sqlite3_step(stmt);
    while (sqlite3_column_text(stmt, 0)) {
        rows.push_back(std::vector<std::string>());
        for (int i = 0; i < colCount; ++i) {
            rows[rowInd].push_back(std::string((char*)sqlite3_column_text(stmt, i)));
        }
        rowInd++;
        sqlite3_step(stmt);
    }

    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            std::cout << rows[i][j] <<  " " ;
        }
        std::cout << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return rows;
}

void MainWindow::handleQuery(){
    // exceptions
    QString isletmeAdi = ui->isletmeAdiLineEdit->text();
    QString tarih = ui->tarihLineEdit->text();

    std::string sIsletmeAdi = isletmeAdi.toUtf8().constData();
    std::string sTarih = tarih.toUtf8().constData();

    // parametreler sql'e nasıl bind edilmeli ?
    // boost yada string stream ile parametreler sqlE2 bind edilebilir
    // isletmeAdini direkt uppercase yap
    if(!isletmeAdi.isEmpty() || !tarih.isEmpty()){
        //widgeti temizle
        int rowCount = ui->fisTableWidget->rowCount();
        for (int var = 0; var < rowCount; ++var) {
            ui->fisTableWidget->removeRow(0);
        }
        if(isletmeAdi.isEmpty()){
            std::cout << "isletme adi yok" << std::endl;
            // sqlite tarih sorgkularını anla

            std::string param = sTarih;
            //std::cout << sIsletmeAdi << "," << sTarih << std::endl;

            std::string sql = "SELECT I.TICARI_UNVANI, F.TARIHI, F.FIS_NO,SUM(U.KDV + U.FIYAT) AS TOPLAM ,SUM(U.KDV) AS KDV\
            FROM FIS AS F\
            INNER JOIN URUNLER AS U ON U.FIS_ID = F.FIS_ID\
            INNER JOIN ISLETME AS I ON I.ISLETME_ID = F.ISLETME_ID\
            WHERE F.TARIHI = '%1%'\
            GROUP BY F.FIS_ID";

            std::string bSql = boost::str( boost::format(sql) % param );
            //std::cout << bSql << std::endl;
            char *qsql= (char *)bSql.c_str();
            //std::cout << qsql << std::endl;

            std::vector<std::vector<std::string>> rows = query(DB,5,qsql);
            // yeni veriler gelmeden eski verileri sil en optimum degil ama is gorur
            int rowCount = ui->fisTableWidget->rowCount();

            for (int var = 0; var < rowCount; ++var) {
                ui->fisTableWidget->removeRow(0);
            }
            for(int i = 0; i < (int)rows.size(); i++){
                ui->fisTableWidget->insertRow(ui->fisTableWidget->rowCount());
                for(int j = 0; j < (int)rows[i].size(); j++)
                {
                    QTableWidgetItem *item = ui->fisTableWidget->item(i, j);
                    if(!item) {
                        item = new QTableWidgetItem();
                        ui->fisTableWidget->setItem(i, j, item);
                        //std::cout << rows[i][j];
                    }
                    QString tmp = QString::fromStdString(rows[i][j]);
                    item->setText(tmp);
                }
            }
            ui->retranslateUi(this);
        }
        else if(tarih.isEmpty()){
            //std::cout << "tarih adi yok" << std::endl;
            std::string param = sIsletmeAdi;
            //std::cout << sIsletmeAdi << "," << sTarih << std::endl;

            std::string sql = "SELECT I.TICARI_UNVANI, F.TARIHI, F.FIS_NO,SUM(U.KDV + U.FIYAT) AS TOPLAM ,SUM(U.KDV) AS KDV\
            FROM FIS AS F\
            INNER JOIN URUNLER AS U ON U.FIS_ID = F.FIS_ID\
            INNER JOIN ISLETME AS I ON I.ISLETME_ID = F.ISLETME_ID\
            WHERE I.TICARI_UNVANI = '%1%'\
            GROUP BY F.FIS_ID";

            std::string bSql = boost::str( boost::format(sql) % param );
            //std::cout << bSql << std::endl;
            char *qsql= (char *)bSql.c_str();
            //std::cout << qsql << std::endl;

            std::vector<std::vector<std::string>> rows = query(DB,5,qsql);
            // yeni veriler gelmeden eski verileri sil en optimum degil ama is gorur
            int rowCount = ui->fisTableWidget->rowCount();

            for (int var = 0; var < rowCount; ++var) {
                ui->fisTableWidget->removeRow(0);
            }
            for(int i = 0; i < (int)rows.size(); i++){
                ui->fisTableWidget->insertRow(ui->fisTableWidget->rowCount());
                for(int j = 0; j < (int)rows[i].size(); j++)
                {
                    QTableWidgetItem *item = ui->fisTableWidget->item(i, j);
                    if(!item) {
                        item = new QTableWidgetItem();
                        ui->fisTableWidget->setItem(i, j, item);
                        //std::cout << rows[i][j];
                    }
                    QString tmp = QString::fromStdString(rows[i][j]);
                    item->setText(tmp);
                }
            }
            ui->retranslateUi(this);

        }
        else if (!isletmeAdi.isEmpty() && !tarih.isEmpty()){
            //std::cout << "ikiside dolu" << std::endl;
            // 2 parametreyide gecir

            std::string param1 = sIsletmeAdi;
            std::string param2 = sTarih;
            //std::cout << sIsletmeAdi << "," << sTarih << std::endl;

            std::string sql = "SELECT I.TICARI_UNVANI, F.TARIHI, F.FIS_NO,SUM(U.KDV + U.FIYAT) AS TOPLAM ,SUM(U.KDV) AS KDV\
            FROM FIS AS F\
            INNER JOIN URUNLER AS U ON U.FIS_ID = F.FIS_ID\
            INNER JOIN ISLETME AS I ON I.ISLETME_ID = F.ISLETME_ID\
            WHERE I.TICARI_UNVANI = '%1%' AND F.TARIHI = '%2%'\
            GROUP BY F.FIS_ID";

            std::string bSql = boost::str( boost::format(sql) % param1 % param2 );
            //std::cout << bSql << std::endl;
            char *qsql= (char *)bSql.c_str();
            //std::cout << qsql << std::endl;

            std::vector<std::vector<std::string>> rows = query(DB,5,qsql);
            // yeni veriler gelmeden eski verileri sil en optimum degil ama is gorur
            int rowCount = ui->fisTableWidget->rowCount();

            for (int var = 0; var < rowCount; ++var) {
                ui->fisTableWidget->removeRow(0);
            }
            for(int i = 0; i < (int)rows.size(); i++){
                ui->fisTableWidget->insertRow(ui->fisTableWidget->rowCount());
                for(int j = 0; j < (int)rows[i].size(); j++)
                {
                    QTableWidgetItem *item = ui->fisTableWidget->item(i, j);
                    if(!item) {
                        item = new QTableWidgetItem();
                        ui->fisTableWidget->setItem(i, j, item);
                        //std::cout << rows[i][j];
                    }
                    QString tmp = QString::fromStdString(rows[i][j]);
                    item->setText(tmp);
                }
            }
            ui->retranslateUi(this);
        }
    }
    else {
        char* sql = "SELECT I.TICARI_UNVANI, F.TARIHI, F.FIS_NO,SUM(U.KDV + U.FIYAT) AS TOPLAM ,SUM(U.KDV) AS KDV\
                FROM FIS AS F\
                INNER JOIN URUNLER AS U ON U.FIS_ID = F.FIS_ID\
                INNER JOIN ISLETME AS I ON I.ISLETME_ID = F.ISLETME_ID\
                GROUP BY F.FIS_ID\;";
        std::vector<std::vector<std::string>> rows = query(DB,5,sql);
        // yeni veriler gelmeden eski verileri sil en optimum degil ama is gorur
        int rowCount = ui->fisTableWidget->rowCount();
        for (int var = 0; var < rowCount; ++var) {
            ui->fisTableWidget->removeRow(0);
        }

        for(int i = 0; i < (int)rows.size(); i++){
            ui->fisTableWidget->insertRow(ui->fisTableWidget->rowCount());
            for(int j = 0; j < (int)rows[i].size(); j++)
            {
                QTableWidgetItem *item = ui->fisTableWidget->item(i, j);
                if(!item) {
                    item = new QTableWidgetItem();
                    ui->fisTableWidget->setItem(i, j, item);
                    //std::cout << rows[i][j];
                }
                QString tmp = QString::fromStdString(rows[i][j]);
                item->setText(tmp);
            }
        }
        ui->retranslateUi(this);

    }
}

void MainWindow::handleFisAra(){
    QString qFisNo = ui->fisLineEdit->text();

    if(!qFisNo.isEmpty()){
        std::string fisNo = qFisNo.toUtf8().constData();
        //std::cout << fisNo << std::endl;

        int rowCount = ui->urunlerTableWidget->rowCount();
        for (int var = 0; var < rowCount; ++var) {
            ui->urunlerTableWidget->removeRow(0);
        }

        std::string param1 = fisNo;
        // fisno dan fis id ye ulas
        std::string sql1 = "SELECT FIS_ID FROM FIS WHERE FIS_NO = %1%";

        std::string bSql1 = boost::str( boost::format(sql1) % param1 );
        char *qsql1= (char *)bSql1.c_str();
        std::vector<std::vector<std::string>> fisIdRow = query(DB,1,qsql1);
        //std::cout<< fisIdRow[0][0]  << std::endl;

        std::string fisID = fisIdRow[0][0];
        //std::cout << fisIdRow[0][0] << std::endl;

        std::string param = fisID;
        std::string sql = "SELECT FIYAT, KDV, URUN_ADI FROM URUNLER WHERE FIS_ID = %1%";
        //std::cout<< sql << std::endl;

        std::string bSql = boost::str( boost::format(sql) % param );
        char *qsql= (char *)bSql.c_str();

        std::vector<std::vector<std::string>> rows = query(DB,3,qsql);

        for(int i = 0; i < (int)rows.size(); i++){
            ui->urunlerTableWidget->insertRow(ui->urunlerTableWidget->rowCount());
            for(int j = 0; j < (int)rows[i].size(); j++)
            {
                QTableWidgetItem *item = ui->urunlerTableWidget->item(i, j);
                if(!item) {
                    item = new QTableWidgetItem();
                    ui->urunlerTableWidget->setItem(i, j, item);
                    //std::cout << rows[i][j];
                }
                QString tmp = QString::fromStdString(rows[i][j]);
                item->setText(tmp);
            }
        }
        ui->retranslateUi(this);
    }


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // qt nin kendi veritabanı cozumude kullanılabilir
    // database baglantısını yap
    int sqlite_init_status = sqlite3_open("/home/mrk0/gelistirme/yaz1lab1/yl.db", &DB);

    //bunu try excepte cevirsek daha iyi degil mi ?
    if (sqlite_init_status ) {
        std::cout << "veri tabanı acilamadi" << std::endl;
        exit(1);
    }
    sqlite3_close(DB);

    ui->setupUi(this);
     // /home/mrk0/gelistirme/yaz1lab1/ditsmachsen.jpeg
    // /home/mrk0/gelistirme/yaz1lab1/fis.jpg
    QPixmap pm("/home/mrk0/gelistirme/yaz1lab1/ditsmachsen.jpeg");
    ui->inpImageLabel->setPixmap(pm);
    ui->inpImageLabel->setScaledContents(true);
    // signal ve slot bindinglerini buraya yap

    // celldoubleclicked open dialog and show product
    //data bekledigim gibi degil
    connect(ui->filePathButton,SIGNAL(released()),this,SLOT(handleFileButton()));
    connect(ui->araButton,SIGNAL(released()),this,SLOT(handleQuery()));
    connect(ui->fisAraButton,SIGNAL(released()),this,SLOT(handleFisAra()));
}

/*
void MyClass::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
    }
}
*/

MainWindow::~MainWindow()
{

    delete ui;
}
