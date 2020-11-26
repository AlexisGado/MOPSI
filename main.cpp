#include "depth.h"

int main()
{
    try
    {
        StereoImages<byte> statue("statueLeft.ppm","statueRight.ppm");
        statue.computeDisparity();
        cout << "Affichage de la carte de disparité d'images en noir et blanc";
        statue.displayAll();
        cout << "Calcul de la carte de disparité d'images en couleur";
        StereoImages<RGB<byte>> aloe("aloeL.png","aloeR.png");
        aloe.computeDisparity();

        click();
        aloe.displayAll();
    }
    catch (char const* err)
    {
        cout<<err<<endl;
    }  
    return 0;
}
