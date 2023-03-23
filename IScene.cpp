#include "IScene.h"
#include "Processing/IPFlow.h"
#include <QDebug>
#include <tuple>
//----------------------------------------------------------------------------
void IScene::on_pbnFitScene_clicked()
{

}

//------------------------------------------------------------------------------
std :: tuple <uint32_t,bool> IScene::processFileName(QString strFileName, QImage & m_imgOut, QImage & HistRgb, bool bSaveImage)
{
    IMat  mImatGray;

    QByteArray ba = strFileName.toLocal8Bit();
    m_imgOut = QImage();
    bool bImgOk = IPFlow::openRaw(ba.data(),mImatGray) ;
    uint32_t sum=0;

    if( bImgOk == true)
    {
        //    applyLut(mImatGray);
        sum = processImg(mImatGray, HistRgb);
        if(bSaveImage==true)
        {
        convertCvrgb2Qimage(mImatGray , m_imgOut);
        mImatGray.release();
        QImage imgPreview = m_imgOut.scaled(QSize(256,256),Qt::KeepAspectRatio);
        QString strIconName = strFileName;
        strIconName.replace(".raw",".png");
        strIconName.replace(".Dat",".png");
        imgPreview.save(strIconName, "png");
        }

    }
    return std::make_tuple(sum,bImgOk);
}
//------------------------------------------------------------------------------

void IScene::convertCvrgb2Qimage(const IMat & mDstRgb, QImage & m_imgOut)
{
    if ( ! mDstRgb.empty() )
    {
        m_imgOut = QImage();
        const int32_t
                lRows = mDstRgb.rows,
                lCols = mDstRgb.cols;
        // QImage(int width, int height, QImage::Format format)
        m_imgOut = QImage(lCols, lRows, QImage::Format_RGB888);
        uint16_t u16PxlVal;

        for(int32_t ii = 0; ii < lRows; ii++)
        {
            for(int32_t jj = 0; jj < lCols; jj++)
            {
                u16PxlVal = mDstRgb.at<uint16_t>(ii,jj)>>8;
                if(u16PxlVal>255)
                {
                    u16PxlVal = 255;
                }
                uint8_t iB = u16PxlVal; // IPImgProcessing::ku8aR[u16PxlVal];
                uint8_t iG = u16PxlVal; // IPImgProcessing::ku8aG[u16PxlVal];
                uint8_t iR = u16PxlVal; // IPImgProcessing::ku8aB[u16PxlVal];
                m_imgOut.setPixel( jj, ii, qRgb(iR,iG,iB) );
            }
        }

    }
}

//------------------------------------------------------------------------------
void IScene::applyLut(const IMat & mDstRgb)
{

}

//------------------------------------------------------------------------------
uint32_t IScene::processImg(const IMat & mDstRgb, QImage &HistRgb)
{
    const uint32_t SZ_VETT = 65536;
    const uint32_t SZ_HIST = 256;
    uint16_t u16aLut[65536]={0};
    uint32_t u32aAmount[SZ_VETT]={0};

    float_t fLutVal;

    const int32_t
            lRows = mDstRgb.rows,
            lCols = mDstRgb.cols;
    uint16_t u16PxlVal;

    //create an array with the number of pixel for each gray level

    int32_t lNumPxl = 0;
    for(int32_t ii = lRows*1/4; ii < lRows*3/4; ii+=10)
    {
        for(int32_t jj = lCols*1/4; jj < lCols*3/4; jj+=10)
        {
            u16PxlVal = mDstRgb.at<uint16_t>(ii,jj);
            u32aAmount[u16PxlVal]++;
            lNumPxl++;
        }
    }

    uint32_t u32aFullHist[SZ_HIST] = {0};

    //create an histogram array with 256 columns

    for(int32_t ii = lRows * 1/4; ii < lRows * 3/4; ii++)
    {
        for(int32_t jj = lCols * 1/4; jj < lCols * 3/4; jj++)
        {
            u16PxlVal = mDstRgb.at<uint16_t>(ii,jj);
            u16PxlVal = u16PxlVal >> 8;
            u32aFullHist[u16PxlVal] =  u32aFullHist[u16PxlVal] + 1;
        }
    }

    //find max for normalitation

    uint32_t lMax = 0;
    for(uint32_t ii=1; ii<=SZ_HIST; ii++)
    {
        if(u32aFullHist[ii] > lMax)
        {
            lMax = u32aFullHist[ii];
        }
    }

    //create a Qimage for representing the histogramm

    if ( ! mDstRgb.empty() )
    {
        HistRgb = QImage();
        const int32_t
                lRows = 301,
                lCols = SZ_HIST;

        HistRgb = QImage(lCols, lRows, QImage::Format_RGB888);
        uint16_t u16PxlVal;


        for(uint32_t jj = 0; jj < lCols; jj++)
        {
            uint32_t uHistVal = u32aFullHist[jj] * 301 / lMax;
            for(uint32_t ii = 0; ii < lRows; ii++)
            {
                if(ii >= 301- uHistVal)
                {
                    u16PxlVal = 255;
                }
                else
                {
                    u16PxlVal = 128;
                }
                uint8_t iB = u16PxlVal; // IPImgProcessing::ku8aR[u16PxlVal];
                uint8_t iG = u16PxlVal; // IPImgProcessing::ku8aG[u16PxlVal];
                uint8_t iR = u16PxlVal; // IPImgProcessing::ku8aB[u16PxlVal];
                HistRgb.setPixel( jj, ii, qRgb(iR,iG,iB) );
            }
        }

    }


    //Find max treshold and min treshold

    uint32_t lMaxIndx = 0, lMinIndx = 0;
    float fVal, PercMax= 0.0;

    for(uint32_t ii = 1; ii < SZ_VETT; ii++)
    {
        if(u32aAmount[ii]>1){
            lMaxIndx = ii;
        }
        if(lMaxIndx==0){
            lMinIndx=ii;
        }
    }
    PercMax= float(lNumPxl) / float(lMaxIndx-lMinIndx);
    PercMax= (PercMax * 4.7f) + 68;

    lMaxIndx = SZ_VETT-1,lMinIndx = 0;

    const float xfMinThreshold = (3.0f * 0.01f * lNumPxl);
    const float xfMaxThreshold = (PercMax* 0.01f * lNumPxl);
    u32aAmount[0] = 0;
    u32aAmount[SZ_VETT-1] = 65535;
    float fHistGain = 65535.0f/lNumPxl;

    //Equalization
    u16aLut[0] = 65535;
    for(uint32_t ii = 1; ii < SZ_VETT; ii++)
    { // integral
        u32aAmount[ii] = u32aAmount[ii] + u32aAmount[ii-1];
        fVal = static_cast<float> ( u32aAmount[ii] );
        if( fVal <= xfMinThreshold )
        {
            lMinIndx = ii;
        }
        if( fVal <= xfMaxThreshold )
        {
            lMaxIndx = ii;
        }

        fVal  = fVal * fHistGain;

        if( fVal < 0.0f )
        {
            u16aLut[ii] =  0;
        }
        else if(fVal > 65535.0f)
        {
            u16aLut[ii] = 65535;
        }
        else{
            u16aLut[ii] = 65535 - static_cast<uint16_t> (fVal);
        }
    }

    if(0)
    {
        //Fill the array for LUT
        const float fGain = 65536.0f / SZ_VETT; // to manage hist dimension
        float fM =  fGain * (65535.0f - 0.0f) / (lMaxIndx-lMinIndx);

        for(uint32_t ii = 0; ii < 65536; ii++)
        {
            fLutVal = static_cast<float>( ii) - static_cast<float>( lMinIndx );
            fLutVal = fLutVal * ( fM );
            if( fLutVal < 0.0f )
            {
                u16aLut[ii] =  0;
            }
            else if(fLutVal > 65535.0f)
            {
                u16aLut[ii] = 65535;
            }
            else
            {
                u16aLut[ii] = static_cast<uint16_t> (fLutVal);
            }

            u16aLut[ii] = 65535 -  u16aLut[ii]; // negative it !
        }
    }

    //Change value of pixels
    uint32_t sum = 5381;
    for(int32_t ii = 0; ii < lRows; ii++)
    {
        for(int32_t jj = 0; jj < lCols; jj++)
        {
            u16PxlVal = mDstRgb.at<uint16_t>(ii,jj);
            u16PxlVal = u16aLut[u16PxlVal];
            mDstRgb.at<uint16_t>(ii,jj) = u16PxlVal;
            sum = ((sum << 5) + sum) ^ static_cast<uint32_t>( u16PxlVal);
        }
    }

    return(sum);

}
