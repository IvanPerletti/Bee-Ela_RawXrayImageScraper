#include "secdialog.h"
#include "ui_secdialog.h"
#include <QFileDialog>
#include <QMessageBox>

SecDialog::SecDialog(const QStringList &FullFileName, QList <uint32_t> Idlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);
    for(int ii=0; ii<FullFileName.size(); ii++)
    {
        ui->tableWidget->insertRow(ii);

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText(QString::number(Idlist.at(ii)));
        ui->tableWidget->setItem(ii, 0, item2);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(FullFileName.at(ii));
        ui->tableWidget->setItem(ii, 1, item);
        ui->tableWidget->verticalHeader()->sizeHint();
    }



}

SecDialog::~SecDialog()
{
    delete ui;
}

void SecDialog::on_PbnCopy_clicked()
{
    QString FullFileName;
    QString FullFileNamePng;

    int Row = ui->tableWidget->currentRow();
    FullFileName = ui->tableWidget->item(Row, 1)->text();
    FullFileNamePng = FullFileName;
    FullFileNamePng.replace(".Dat", ".Png");

    QFileInfo File(FullFileName);
    QFileInfo FilePng(FullFileNamePng);

    QString FileName = File.fileName();
    QString FileNamePng = FilePng.fileName();

    bool bCtrlCopy;
    QString FldrOut;
    FldrOut = QFileDialog :: getExistingDirectory (this,"open a file", "C://");

    QString NewFileName = FldrOut + "/" + FileName;
    QString NewFileNamePng = FldrOut + "/" + FileNamePng;

    bCtrlCopy = QFile::copy(FullFileName, NewFileName);

    if (bCtrlCopy)
    {
        QFile::copy(FullFileNamePng, NewFileNamePng);
    }
    else
    {
        QMessageBox :: warning(this, "Error", "File non copiato");
    }
}


void SecDialog::on_PbnMove_clicked()
{
    QString FullFileName;
    QString FullFileNamePng;

    int Row = ui->tableWidget->currentRow();
    FullFileName = ui->tableWidget->item(Row, 1)->text();
    FullFileNamePng = FullFileName;
    FullFileNamePng.replace(".Dat", ".Png");

    QFileInfo File(FullFileName);
    QFileInfo FilePng(FullFileNamePng);

    QString FileName = File.fileName();
    QString FileNamePng = FilePng.fileName();

    bool bCtrlCopy;
    QString FldrOut;
    FldrOut = QFileDialog :: getExistingDirectory (this,"open a file", "C://");

    QString NewFileName = FldrOut + "/" + FileName;
    QString NewFileNamePng = FldrOut + "/" + FileNamePng;

    bCtrlCopy = QFile::copy(FullFileName, NewFileName);

    QFile DeleteFile(FullFileName);
    QFile DeleteFilePng(FullFileNamePng);

    if (bCtrlCopy)
    {
        QFile::copy(FullFileNamePng, NewFileNamePng);
        DeleteFile.remove();
        DeleteFilePng.remove();
    }
    else
    {
        QMessageBox :: warning(this, "Error", "File non spostato");
    }

}


void SecDialog::on_PbnDelete_clicked()
{
    QString FullFileName;
    QString FullFileNamePng;

    int Row = ui->tableWidget->currentRow();
    FullFileName = ui->tableWidget->item(Row, 1)->text();
    FullFileNamePng = FullFileName;
    FullFileNamePng.replace(".Dat", ".Png");

    QFile DeleteFile (FullFileName);
    QFile DeleteFilePng (FullFileName);

    bool bCtrl = false;
    bCtrl = DeleteFile.remove();
    DeleteFilePng.remove();
    if(!bCtrl)
    {
        QMessageBox :: warning(this, "Error", "File non rimosso");
    }

}

