#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QFileDialog"
#include "QMessageBox"
#include "QDir"
#include "QTextStream"
#include "QTextEdit"
#include "QPlainTextEdit"
#include "IScene.h"
#include <QKeyEvent>
#include <QWidget>
#include <QtSql>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    QString strFileFullNameRaw = listfilepath.at(currentRow);
    QImage m_imgOut, HistRgb(2048, 1000, QImage::Format_Grayscale8);
    QString strFileFullNamepng = strFileFullNameRaw;
    strFileFullNamepng.replace(".Dat", ".png");
    QFile filePng (strFileFullNamepng);
    QFile fileRaw(strFileFullNameRaw);
    QPixmap pix = QPixmap(":/iconNoImage.png"); // worst case
    QPixmap pixHist;
    pixHist.fill(Qt :: white);
    bool bSaveImage = false;

    // yes png, show png

    if(filePng.exists()==true)
    {
        bSaveImage = false;
        uint32_t sum=0;
        bool bProcessOk=false;
        pix = QPixmap(strFileFullNamepng);
        std::tie(sum,bProcessOk) = IScene :: processFileName(strFileFullNameRaw, m_imgOut, HistRgb, bSaveImage);
        if( bProcessOk == true )
        {
            pixHist = QPixmap :: fromImage (HistRgb);
        }
    }
    else
    { // no png, need to process .Dat
        if( fileRaw.exists() )
        {
            bSaveImage = true;
            uint32_t sum=0;
            bool bProcessOk=false;
            std::tie(sum,bProcessOk)  = IScene :: processFileName(strFileFullNameRaw, m_imgOut, HistRgb,bSaveImage);
            ///  bool bImgOk = IScene :: processFileName(strFileFullName, m_imgOut);

            if( bProcessOk == true && m_imgOut.size().height() > 100 )
            {
                pix = QPixmap::fromImage(m_imgOut);
                pixHist = QPixmap :: fromImage (HistRgb);
            }
        }
    }


    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    ui->HistLabel->setPixmap(pixHist);
    ui->HistLabel->setScaledContents(true);

    ui->label_2->setText("Premi F2 per copiare");


}

//------------------------------------------------------------------------------

//Copy selected png file in a folder

void MainWindow :: keyPressEvent(QKeyEvent *event)
{
    QString OutputPath = ui->OutputPath->toPlainText();
    OutputPath.replace("\\" , "/");
    OutputPath.remove('"');

    if(ui->listWidget->currentItem())
    {
        if(event->key() == Qt::Key_F2)
        {
            QString text = ui->listWidget->currentItem()->text();
            text.replace(".Dat", ".png");

            bool bCopySuccesfull = false;

            QString strFileFullNamepng = listfilepath.at(ui->listWidget->currentRow());
            strFileFullNamepng.replace(".Dat", ".png");

            bCopySuccesfull = QFile::copy(strFileFullNamepng, OutputPath + "/" + text );

            if(bCopySuccesfull==false)
            {

                QMessageBox :: warning(this, "Error", text + "\n File non copiato");

            }
        }
    }

}


//------------------------------------------------------------------------------

// Push button for processing all .dat

void MainWindow::on_pbnProcess_clicked()
{
    listfilepath.clear();
    listdirpath.clear();
    dirinfolist.clear();
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);
    index=0, a=0, x=0;
    int error=0;
    QString InputPath = ui->InputPath->toPlainText();
    InputPath.replace("\\" , "/");
    InputPath.remove('"');
    listdirpath.append(InputPath);
    bool bSaveImage = false;
    uint32_t sum = 0;

    //create a database

    QFile FileDb ("C:/Users/Utente/Desktop/qt/GestioneArchivio/Images Database.sqlite");
    if(FileDb.exists())
    {
        FileDb.remove();
    }
    DbImages = QSqlDatabase :: addDatabase("QSQLITE");
    DbImages.setDatabaseName("C:/Users/Utente/Desktop/qt/GestioneArchivio/Images Database.sqlite");
    if(!DbImages.open())
    {
     qDebug()<<"Database not open";
    }

    QString Query = "CREATE TABLE Images ("
                          "FileName string,"
                          "FilePath string,"
                          "Id uint32_t);";

    QSqlQuery DatabaseQuery;
    if(!DatabaseQuery.exec(Query))
    {
        qDebug()<<"Error creating table";
    }

    //for each folder control if contains other folders

    for(int ii=0; ii<index+1 ; ii++){

        QDir Dir(listdirpath[x]);

        listfilenameTemp = Dir.entryList(QDir :: Files);

        Dir.setFilter(QDir :: Files | QDir :: NoDotAndDotDot);
        QImage m_imgOut,HistRgb;
        for(int i=0; i<listfilenameTemp.size(); i++)
        { //control if the folder contains .Dat
            if(listfilenameTemp.at(i).endsWith(".Dat"))
            {
                bSaveImage = true;
                listfilepath.append(Dir.filePath(listfilenameTemp.at(i)));

                QString strFullFileName = listfilepath.at(error);
                bool bProcessOk = false;

                std::tie(sum,bProcessOk) = IScene :: processFileName(strFullFileName, m_imgOut, HistRgb, bSaveImage);
                m_imgOut = QImage();
                ui->listWidget->addItem(listfilenameTemp.at(i).toLatin1());
                error++;

                AddValues(listfilenameTemp.at(i), listdirpath[x], sum);
            }
        }

        dirinfolist = Dir.entryInfoList(QDir :: Dirs | QDir :: NoDotAndDotDot);
        if(x==index){
            for(int ii=0; ii<dirinfolist.size(); ii++){

                listdirpath.append(dirinfolist.at(ii).filePath());
                index++;
            }
        }

        x++;


    }

    if(error==0){
        QMessageBox :: warning(this, "Error", "Nessun file .Dat trovato");
    }

    //find duplicates images

    QString strQuery477 =
    " select *                       "
    "   from Images a                "
    "  where ( Id  ) in              "
    "        ( select  Id            "
    "            from Images         "
    "           group by  Id         "
    "          having count(Id) > 1 )" ;

    QSqlQuery DbDuplicatesQuery;
    DbDuplicatesQuery.exec(strQuery477);

    QStringList FullFileName;
    QList <uint32_t> IdList;
    while (DbDuplicatesQuery.next())
    {
        IdList.append(DbDuplicatesQuery.value(2).toInt());
        FullFileName.append(DbDuplicatesQuery.value(1).toString() + "/" + DbDuplicatesQuery.value(0).toString());
    }

    DbImages.close();
}

//------------------------------------------------------------------------------

// Push button for refresh, no process if it is not needed

void MainWindow::on_pbnRefresh_clicked()
{

    listdirpath.clear();
    dirinfolist.clear();
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);
    index=0, a=0, x=0;
    int error=0;
    QString InputPath = ui->InputPath->toPlainText();
    InputPath.replace("\\" , "/");
    InputPath.remove('"');
    listdirpath.append(InputPath);
    bool bSaveImage = false;

    //for each folder control if contains other folders
    for(int ii=0; ii<index+1 ; ii++){

        QDir Dir(listdirpath[x]);

        listfilenameTemp = Dir.entryList(QDir :: Files);

        Dir.setFilter(QDir :: Files | QDir :: NoDotAndDotDot);
        for(int i=0; i<listfilenameTemp.size(); i++)
        { //control if the folder contains .Dat
            if(listfilenameTemp.at(i).endsWith(".Dat"))
            {
                listfilepath.append(Dir.filePath(listfilenameTemp.at(i)));

                QString strFileFullNameRaw = listfilepath.at(error);
                QImage m_imgOut, HistRgb(2048, 1000, QImage::Format_Grayscale8);
                QString strFileFullNamepng = strFileFullNameRaw;
                strFileFullNamepng.replace(".Dat", ".png");
                QFile filePng (strFileFullNamepng);
                QFile fileRaw (strFileFullNameRaw);

                // no png, need to process

                if(filePng.exists()==false && fileRaw.exists()==true)
                {
                    bSaveImage = true;
                    IScene :: processFileName(strFileFullNameRaw, m_imgOut, HistRgb, bSaveImage);
                }

                m_imgOut = QImage();
                ui->listWidget->addItem(listfilenameTemp.at(i).toLatin1());
                error++;


            }
        }

        dirinfolist = Dir.entryInfoList(QDir :: Dirs | QDir :: NoDotAndDotDot);
        if(x==index){
            for(int jj=0; jj<dirinfolist.size(); jj++){

                listdirpath.append(dirinfolist.at(jj).filePath());
                index++;
            }
        }

        x++;


    }

    if(error==0){
        QMessageBox :: warning(this, "Error", "Nessun file .Dat trovato");
    }

}






//------------------------------------------------------------------------------

void MainWindow :: AddValues(QString FileName, QString FilePath, double Id)
{
    QSqlQuery DatabaseQuery;

    DatabaseQuery.prepare("INSERT INTO Images ("
                          "FileName,"
                          "FilePath,"
                          "Id)"
                          "Values (?,?,?);");




    DatabaseQuery.addBindValue(FileName);
    DatabaseQuery.addBindValue(FilePath);
    DatabaseQuery.addBindValue(Id);

    if(!DatabaseQuery.exec())
    {
        qDebug()<<"Error adding values to db";
    }

}
