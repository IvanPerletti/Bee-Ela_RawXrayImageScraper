#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStringList>
#include <QFileInfoList>
#include <QListWidgetItem>
#include <QtSql>
#include <IScene.h>


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

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pbnProcess_clicked();

    void on_pbnRefresh_clicked();

    void keyPressEvent(QKeyEvent *event);

    void AddValues(QString FileName, QString FilePath, double Id);

private:
    Ui::MainWindow *ui;

    QStringList listdirpath;
    QStringList listfilenameTemp;
    QFileInfoList dirinfolist;
    int index=0, a=0, x=0;
    QStringList listfilepath;

    QSqlDatabase DbImages;
};
#endif // MAINWINDOW_H
