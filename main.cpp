#include "depth.h"

int main()
{
    try
    {
        /*StereoImages<byte> statue("statueL.ppm","statueR.ppm", false, true);
        cout << "Calculs statue en cours..." << endl;
        statue.computeDisparity(true);
        statue.displayAll();*/

        /*StereoImages<RGB<byte>> aloe("AloeG/view1.png","AloeG/view5.png", true, false);
        //StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png", false, false);
        cout << "Calculs aloe en cours..." << endl;
        aloe.computeDisparity(true);
        aloe.computeScore("AloeG/disp1.png","AloeG/disp5.png");
        cout << "Cliquez sur l'image" << endl;
        aloe.displayAll();*/

        /*StereoImages<byte> cones("cones2.png","cones6.png", false, false);
        cout << "Calculs cones en cours..." << endl;
        cones.computeDisparity(true);
        cout << "Cliquez sur l'image" << endl;
        cones.displayAll();*/

        /*StereoImages<byte> bowling("Bowling1/view1.png","Bowling1/view5.png", true, true);
        cout << "Calculs bowling en cours..." << endl;
        bowling.computeDisparity(true);
        bowling.computeScore("Bowling1/disp1.png","Bowling1/disp5.png");
        cout << "Cliquez sur l'image" << endl;
        bowling.displayAll();*/

        StereoImages<byte> Monopoly("Monopoly/view1.png","Monopoly/view5.png", true, true);
        cout << "Calculs Monopoly en cours..." << endl;
        Monopoly.computeDisparity(true);
        Monopoly.computeScore("Monopoly/disp1.png","Monopoly/disp5.png");
        cout << "Cliquez sur l'image" << endl;
        Monopoly.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
