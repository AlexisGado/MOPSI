#include "depth.h"

int main()
{
    try
    {
        StereoImages<byte> statue("aloeL-petit.png","aloeR-petit.png", false, true);
        cout << "Calculs en cours..." << endl;
        statue.computeDisparity();
        //StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png");
        //aloe.computeDisparity();
        cout << "Cliquez sur l'image" << endl;
        statue.displayAll();
        //aloe.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
