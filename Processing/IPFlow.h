#ifndef IPFLOW_H
#define IPFLOW_H
#include <Processing/IMat.h>
#include <stdint.h>

class IPFlow
{
public:
    typedef enum
    {
        LIB_01_21 = 121,
        LIB_02_01 = 201,
        LIB_02_02 = 202,
        LIB_03_01 = 301,
    } enumLib;

    typedef struct struct_Param
    {
        // Family 1xx - GMM processing
        // Family 2xx - smart Collimator adjustment
        // Family 7xx - Final Lut related
        enumLib enVersion   =   LIB_03_01;
        int32_t lParam_100  =   100    ;     // automatic Lut
        int32_t lParam_101  =   5      ;     // Shutter
        int32_t lParam_102  =   5      ;     // Details
        int32_t lParam_103  =   65     ;     // Contrast
        int32_t lParam_104  =   51     ;     // noise removal
        int32_t lParam_105  =   10     ;     // gloom
        int32_t lParam_106  =   50     ;     // not used
        int32_t lParam_107  =   50     ;     // not used
        int32_t lParam_108  =   50     ;     // not used
        int32_t lParam_209  =   1      ;     // automatic collimator crop
        int32_t lParam_700  =   0      ;     // lMinLutLinear - Linear Lut min
        int32_t lParam_701  =   65535  ;     // lMaxLutLinear - Linear Lut max
        int32_t lParam_702  =   0      ;     // lMinLutParam  - Parametric curve Lut local min
        int32_t lParam_703  =   65535  ;     // lMaxLutParam  - Parametric curve Lut local max
    } strParam;

public:
    IPFlow();
    static bool openRaw(const char *caFileName, IMat &mDst, const unsigned long lShift=256, bool bIsVarex = false);
    static void ardega(const char u8aFileName[], IMat &mProcessed);
    static void execute(const IMat &mRawIn, IMat &mOut);
private:
    static void enhance(const IMat &mRawIn, IMat &mOut);
    static void applyLog(const IMat &mSrc, IMat &mDst, const int32_t lLogSlope);
    static void getLog(uint16_t *u16aLUT, const int32_t lSzVett, int32_t maxValue);
};

#endif // IPFLOW_H
