#include "depth.h"

int main()
{
    try
    {
        StereoImages statue("statueLeft.ppm","statueRight.ppm");
        statue.computeDisparity();
        statue.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}