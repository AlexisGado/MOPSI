#include "depth.h"

int main()
{
    try
    {
        //StereoImages<byte> statue("aloeL.png","aloeR.png", true, true);
        StereoImages<byte> statue("AloeG/view1.png","AloeG/view5.png", true, true);
        cout << "Calculs statue en cours..." << endl;
        statue.computeDisparity();
        statue.computeScore("AloeG/disp1.png","AloeG/disp5.png");
        statue.displayAll();

        /*StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png", false, false);
        cout << "Calculs aloe en cours..." << endl;
        aloe.computeDisparity();
        cout << "Cliquez sur l'image" << endl;
        aloe.displayAll();*/

        /*StereoImages<byte> cones("cones2.png","cones6.png", false, false);
        cout << "Calculs cones en cours..." << endl;
        cones.computeDisparity();
        cout << "Cliquez sur l'image" << endl;
        cones.displayAll();*/
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
