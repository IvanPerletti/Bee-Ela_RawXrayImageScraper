#include "IMat.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <QDebug>
#include <algorithm>



//----------------------------------------------------------------------------
IMat::IMat(void)
    : rows(0), cols(0), data(nullptr)
{

}
//----------------------------------------------------------------------------
IMat::IMat(int iImgRows, int iImgCols, int iType)
    : rows(0), cols(0), data(nullptr)
{
    init(iImgRows,iImgCols,iType);
}
//----------------------------------------------------------------------------
void IMat::init(int iImgRows, int iImgCols, int iType)
{
    if ( iImgRows > 0 && iImgCols > 0 )
    {
        rows = iImgRows;
        cols = iImgCols;
        if (data != nullptr )
        {
            delete [] data;
        }
        data =  new uint16_t [iImgRows * iImgCols * iType];
    }
}
//----------------------------------------------------------------------------



////----------------------------------------------------------------------------
//uint16_t & IMat::at(int iRow, int iCol)
//{
//    return (data[rows*iRow + iCol]);
//}

//----------------------------------------------------------------------------
uint16_t & IMat::ad(int iRow, int iCol)
{
    uint16_t & u16Val = data[0];
    if (iRow >= 0 && iCol>= 0 )
    {
        if ( rows > iRow && cols > iCol)
        {
            u16Val = data[rows*iRow + iCol];
        }
    }

    return (u16Val);
}
