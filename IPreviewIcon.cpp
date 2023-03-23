#include "IPreviewIcon.h"
#include "ui_IPreviewIcon.h"
#include <QImage>
//-----------------------------------------------------------------------------
IPreviewIcon::IPreviewIcon(const QString &strIconText ,
                           const QString &strRawPath  ,
                           QWidget       *parent      )
    : ui(new Ui::IPreviewIcon)
{
    mstrIconText = strIconText   ;
    mstrRawPath  = strRawPath;
    ui->setupUi(this);
    QString strPngPath = strRawPath;
    strPngPath.replace(".raw", ".png");
    strPngPath.replace(".Dat", ".png");
    QString strStyleSheet = ".QFrame {border-image: url(" + strPngPath   + ") 0 0 0 0 stretch stretch; }";
    this->setToolTip(strPngPath);
    ui->frmIcon->setStyleSheet(strStyleSheet);
    ui->lblText->setText(strIconText);
    ui->lblStatus->hide();
}
//-----------------------------------------------------------------------------
IPreviewIcon::~IPreviewIcon()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void IPreviewIcon :: setTitle(QString strTitle)
{
    mstrIconText = strTitle;
    ui->lblText->setText(strTitle);
}
//-----------------------------------------------------------------------------
void IPreviewIcon :: setOverlay(int32_t lVal)
{
    switch(lVal)
    {
    case 1:
        ui->lblStatus->show();
        break;
    default:
        ui->lblStatus->hide();
        break;
    }

}
//-----------------------------------------------------------------------------
void IPreviewIcon::mousePressEvent(QMouseEvent *event)
{
    emit clicked(mstrRawPath);
//    this->setFocus();
}
//-----------------------------------------------------------------------------
void IPreviewIcon::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(mstrRawPath);
    this->setFocus();
}
//void IPreviewIcon::paintEvent(QPaintEvent *e)
//{
//    QPainter painter(ui->frmIcon);

//    QPixmap pxMap = mIcon.pixmap(ui->frmIcon->sizeHint());
//    painter.drawPixmap(ui->frmIcon->rect(), pxMap);

//    IPreviewIcon::paintEvent(e);

//}
