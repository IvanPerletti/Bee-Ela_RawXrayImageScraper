#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>

namespace Ui {
class SecDialog;
}

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(const QStringList &FullFileName, QList <uint32_t> Idlist, QWidget *parent = nullptr);
    ~SecDialog();


private slots:
    void on_PbnCopy_clicked();

    void on_PbnMove_clicked();

    void on_PbnDelete_clicked();

private:
    Ui::SecDialog *ui;
};

#endif // SECDIALOG_H
