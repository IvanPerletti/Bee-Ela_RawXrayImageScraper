#ifndef IMAT_H
#define IMAT_H
#include <stdio.h>
#include <memory>
#include <cstdint>

#define CV_16UC1 2

class Size
{
    public:
    Size();
    int32_t rows;
    int32_t cols;
    int32_t height(void) { return (rows);};
    int32_t width (void) { return (cols);};
};


template <class T>
class mypair {
    T * data;
};

class IMat
{
public:
    int32_t rows;
    int32_t cols;
private:

public:
    IMat(void);
    IMat(int iImgRows, int iImgCols, int iType);

    template <typename T>
    T & at(int iRow, int iCol) const
    {
        return ( static_cast<T&>( data[cols*iRow + iCol] ) );
    }
    void init (int iImgRows, int iImgCols, int iType);
    uint16_t &ad(int iRow, int iCol);
    bool empty() const
    {
        return ((rows|cols) == 0);
    }
    void release()
    {
        delete [] data;
    }
    IMat clone() const
    {
        IMat mOut;
        mOut.init(rows, cols, 2);
        memcpy(  mOut.data, this->data, rows*cols*2);
        return mOut;
    }
private:
    uint16_t * data ;
};

#endif // IMAT_H
