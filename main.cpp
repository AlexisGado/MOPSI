#include "depth.h"

int main()
{
    try
    {
        cout << "Calcul de la carte de disparité d'images en noir et blanc";
        StereoImages<byte> statue("statueLeft.ppm","statueRight.ppm");
        statue.computeDisparity();
        cout << "Calcul de la carte de disparité d'images en couleur";
        StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png");
        statue.displayAll();
        aloe.computeDisparity();
        aloe.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
