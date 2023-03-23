#ifndef ISCENE_H
#define ISCENE_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <Processing/IMat.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class IScene
{
	QImage m_imgIn ;
	QString m_strImgName;
	int m_iOffPosItem;

	public:
	IScene();
    static std :: tuple <uint32_t,bool> processFileName(QString strFileName, QImage &m_imgOut, QImage &HistRgb, bool bSaveImage);
	~IScene();

    static void applyLut(const IMat &mDstRgb);
    static uint32_t SumFunction();

private:
    static uint32_t processImg(const IMat & mDstRgb, QImage &HistRgb);

    static void convertCvrgb2Qimage(const IMat &mDstRgb, QImage &m_imgOut);
    static void on_pbnFitScene_clicked();
	
};


#endif // ISCENE_H
