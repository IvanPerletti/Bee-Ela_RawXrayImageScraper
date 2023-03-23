#ifndef IPREVIEWICON_H
#define IPREVIEWICON_H

#include <QWidget>

namespace Ui {
class IPreviewIcon;
}


#include <QFrame>
#include <QPainter>
#include <QIcon>

//class CFrame : public QFrame
//{
//    Q_OBJECT

//public:
//    explicit CFrame(QWidget *parent = nullptr);
//    void paintEvent(QPaintEvent *) override;
//};

//CFrame::CFrame(QWidget *parent) : QFrame(parent)
//{

//}

//void CFrame::paintEvent(QPaintEvent *e)
//{
//    QPainter painter(this);
//    painter.drawImage(this->rect(),QImage("frameImage.png"));
//    QFrame::paintEvent(e);

//}

class IPreviewIcon : public QWidget
{
    Q_OBJECT

public:
    explicit IPreviewIcon(const QString &strIconText,
                          const QString &strRawPath, QWidget *parent = nullptr);
    ~IPreviewIcon();

    void setTitle(QString strTitle);
    void setFileName(QString strTitle);
    //void paintEvent(QPaintEvent *) override;
    void setOverlay(int32_t lVal);
private:
    QString mstrIconText  ;
    QString mstrRawPath   ;
    Ui::IPreviewIcon *ui  ;


signals:
    void clicked(QString strFilePath);
    void doubleClicked(QString strFilePath);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

};




#endif // IPREVIEWICON_H
