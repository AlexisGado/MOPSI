#include "depth.h"

int main()
{
    try
    {
        StereoImages<byte> statue("statueLeft.ppm","statueRight.ppm", false, false);
        cout << "Calculs statue en cours..." << endl;
        statue.computeDisparity(false);
        statue.computeDisparity(true);
        statue.displayAll();

        /*StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png", false, false);
        cout << "Calculs aloe en cours..." << endl;
        aloe.computeDisparity(true);
        cout << "Cliquez sur l'image" << endl;
        aloe.displayAll();

        StereoImages<byte> cones("cones2.png","cones6.png", false, false);
        cout << "Calculs cones en cours..." << endl;
        cones.computeDisparity(true);
        cout << "Cliquez sur l'image" << endl;
        cones.displayAll();*/
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
