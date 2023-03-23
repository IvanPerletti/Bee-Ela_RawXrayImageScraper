#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <Processing/ITimePerform.h>
#include <Processing/IMat.h>
#include "IPFlow.h"

IPFlow::IPFlow()
{

}


//----------------------------------------------------------------------------
/**
 * @brief open a specified raw file
 * @param caFileName	path to file to be opened
 * @param rows		num of rows of raw file
 * @param cols		num of coloumns in the raw file
 * @param shift		elements to be ignored from the beginning of raw file
 * @return Matrix CV_16UC1
 */
bool IPFlow::openRaw(const char * caFileName,
                     IMat & mDst,
                     const unsigned long lShift, bool bIsVarex)
{
    bool bRawOpened = false;
    if ( caFileName == NULL )
    {
        std::cout << "Null Path Name" << std::endl;
    }
    else
    {
        int size = 0;;
        std::ifstream file( caFileName, std::ios::binary | std::ios::ate);
        if (!file.fail())
        {
           size = file.tellg();
        }
        file.close();
        if(size > 1000)
        {
            std::ifstream fs;
            fs.open(caFileName, std::ifstream::in | std::ifstream::binary);
            if( fs.fail() )
            {
                std::cout << "Invalid Path Name" << std::endl;
            }
            else
            {
                uint8_t u8a256[260]{ 0,} ;
                unsigned long s32Cols  ;
                unsigned long s32Rows  ;
                if (!bIsVarex)
                {
                    fs.read((char*)u8a256, 256 );
                    //fs.seekg(256); // Sets the position of the next char to be extracted from the in-stream.
                    //            int a = u8a256[13]<<8;
                    //            int b = u8a256[12];
                    //            int c = u8a256[17]<<8;
                    //            int d = u8a256[16];
                    //            for (int jj = 0; jj< 256; jj++)
                    //            {
                    //                std::cout << u8a256[jj] <<std::endl;
                    //            }
                    //            std::cout<< a   <<  " " << b <<std::endl;
                    //            std::cout<< c   <<  " " << d <<std::endl;

                    s32Cols = ( u8a256[13]<<8 ) +  u8a256[12];
                    s32Rows = ( u8a256[17]<<8 ) +  u8a256[16];
                } else // varex PaxScan
                {
                    s32Cols = 3072;
                    s32Rows = 2476;
                }


                if ( (s32Rows * s32Cols) < (3300 * 3300) )
                {
                    unsigned short *tmp = new unsigned short [s32Rows*s32Cols + lShift];
                    fs.read((char*)tmp, (s32Rows*s32Cols)*2 );
                    mDst.release();
                    mDst = IMat(s32Rows, s32Cols, CV_16UC1);

                    for ( unsigned long i=0; i<s32Rows; i++ )
                    {
                        for ( unsigned long j=0; j<s32Cols; j++ )
                        {
                            mDst.at< unsigned short >(i,j) = tmp[i*s32Cols+j];
                        }
                    }
                    fs.close();
                    delete[] tmp;
                }
                bRawOpened = true;
            }
        }
		// shhhhh  applyLog(mDst,mDst,40000);
    }
    return bRawOpened;
}
//------------------------------------------------------------------------------
/** 
 * @brief get Logarithmic LUT application
 * @param maxValue					   logarithmic slope coefficent sharpening
 * @return array of logaritmic Lut
 */
void IPFlow::getLog(   uint16_t    * u16aLUT   ,
                      const int32_t lSzVett   ,
                      int32_t      maxValue  )
{
    const float fMaxU16 = (1L<<16L)-1;
    float C = (fMaxU16) / log10f(1.0f + maxValue);
    float fY ;
    u16aLUT[0] = 0;
    for ( int32_t i = 1; i < lSzVett-1; ++i )
    {
        fY = roundf ( C * log10f(1.0+i) );
        if (fY<0.0f)
        {
            fY =0.0f;
        }
        else if (fY>65534.0f)
        {
            fY=65534.0f;
        }
        u16aLUT[i] = (uint16_t) (fY);
    }
    u16aLUT[lSzVett-1] = 0xFFfe;
}
//------------------------------------------------------------------------------
/**
 * @brief apply Logarithmic LUT to image
 * @param I     image to be manipulated
 */
void IPFlow::applyLog(const IMat& mSrc, IMat &mDst, const int32_t lLogSlope)
{
    double maxVal = 655.35 * lLogSlope;

    maxVal = lLogSlope ;
    uint16_t LUT [1<<16];
    getLog(LUT, 1<<16 , (int32_t) maxVal);
    //    IPShow::image(mSrc, "Src");
    const int iRows =  mSrc.rows;
    const int iCols =  mSrc.cols;

    if (mDst.rows != iRows || mDst.cols != iCols)
    {
        mDst.release();
        mDst = IMat( mSrc.rows, mSrc.cols, CV_16UC1 );
    }
    uint16_t u16Gray;
    for ( int32_t i = 0; i < iRows; ++i )
    {
        for ( int32_t j = 0; j < iCols; ++j )
        {
            u16Gray = mSrc.at<uint16_t>(i, j) ;
            u16Gray =  LUT [ u16Gray ];
            mDst.at<uint16_t>(i, j) = u16Gray ;
        }
    }

}

//----------------------------------------------------------------------------
void IPFlow::enhance(const IMat &mRawIn, IMat &mOut)
{
    ITimePerform timerProc;
    timerProc.start("Enhance");

    // explicit allocation for debug reason only

    mOut = mRawIn.clone();
}
//----------------------------------------------------------------------------
void IPFlow::execute(const IMat &mRawIn, IMat &mOut)
{
    ITimePerform timerProc;
    timerProc.start("Execute");

    if ( mRawIn.empty() )
    {
        std::cout << "Invalid Path Name" << std::endl;
    }
    else
    {
        enhance(mRawIn , mOut);
    }
}
//----------------------------------------------------------------------------
/**
 * @brief IPFlow::ardega open locally stored image
 * @param u8aFileName  path to image RAW to be processed
 * @param mProcessed   output image to be processed
 * @param stParam      structure with processing parameters
 */
void IPFlow::ardega(const char u8aFileName[] , IMat &mProcessed  )
{
    // explicit allocation for debug reason only
    ITimePerform timerProc;
    timerProc.start("Processing");

    IMat imgRawU16 ;
    openRaw(u8aFileName , imgRawU16,  256);

    execute(imgRawU16, mProcessed);

    imgRawU16   .release();
}


