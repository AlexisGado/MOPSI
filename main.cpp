#include "depth.h"

int main()
{
    try
    {
        StereoImages<byte> statue("statueLeft.ppm","statueRight.ppm");
        statue.computeDisparity();
        StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png");
        aloe.computeDisparity();
        statue.displayAll();
        aloe.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
