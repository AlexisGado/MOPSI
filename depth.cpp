#include "depth.h"


StereoImages::StereoImages(string nameImL, string nameImR)
{
    if (!load(imLeft,srcPath("images")+string("/")+nameImL)) throw "Issue when loading the left image";
    if (!load(imRight,srcPath("images")+string("/")+nameImR)) throw "Issue when loading the right image";

    width = imLeft.width();
    height = imLeft.height();

    dispL = Image<byte>(width,height);
    dispR = Image<byte>(width,height);
}

int StereoImages::dif(int row, int colLeft, int colRight) const
{
    return abs(imLeft(colLeft, row)-imRight(colRight, row));
}

void StereoImages::computeDPMatrix(Matrix<int>& dpMat, int row) const
{
    //Note : dpMat is a square matrix width*width

    dpMat(0,0) = 0;

    for (int x = 1; x<width; x++)
    {
        dpMat(0,x) = dpMat(0,x-1) + dif(row,0,x);
    }
    for (int y = 1; y<width; y++)
    {
        dpMat(y,0) = dpMat(y-1,0) + dif(row,y,0);
    }

    for (int y = 1; y<width; y++)
    {
        for (int x = 1; x<width; x++)
        {
            dpMat(y,x) = min(dpMat(y-1,x-1),min(dpMat(y,x-1),dpMat(y-1,x))) + dif(row,y,x);
        }
    }
}

void StereoImages::addRowDisparityMaps(Matrix<int>& dispMapL, Matrix<int>& dispMapR, int row) const
{
    Matrix<int> dpMat(width,width);
    computeDPMatrix(dpMat, row);

    int x = width-1;
    int y = width-1;

    int up = 0;
    int left = 0;
    int upLeft = 0;
    int minimum = 0;

    dispMapL(row, 0) = 0;
    dispMapR(row, 0) = 0;

    while (x!=0 || y!=0)
    {
        dispMapL(row, y) = x-y;
        dispMapR(row, x) = y-x;
        if (y>0) up = dpMat(x,y-1);
        if (x>0) left = dpMat(x-1,y);
        if (x>0 && y>0) upLeft = dpMat(x-1,y-1);
        minimum = min(up,min(left,upLeft));

        if (y==0) {x--; continue;}
        if (x==0) {y--; continue;}
        if (minimum == upLeft) {y--; x--; continue;} // Add a tolerance in the test if dpMat becomes a float matrix
        if (minimum == up) {y--; continue;}
        if (minimum == left) {x--; continue;}
    }
}

void StereoImages::computeDisparity()
{
    Matrix<int> dispMatrixL(height, width);
    Matrix<int> dispMatrixR(height, width);
    for (int row = 0; row<height; row++)
    {
        addRowDisparityMaps(dispMatrixL, dispMatrixR, row);
    }

    pair<int,int> minmaxL = range(dispMatrixL);
    pair<int,int> minmaxR = range(dispMatrixL);

    int minL = minmaxL.first;
    int minR = minmaxR.first;

    int maxL = minmaxL.second - minL;
    int maxR = minmaxR.second - minR;

    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            //cout<<"check "<<x<<" "<<y<<endl;
            dispL(x,y) = byte(255*float((dispMatrixL(y,x)-minL))/float(maxL));
            dispR(x,y) = byte(255*float((dispMatrixR(y,x)-minR))/float(maxR));
        }
    }
}

void StereoImages::displayAll() const
{
    Window w1 = openWindow(width,height);
    setActiveWindow(w1);
    display(imLeft);

    Window w2 = openWindow(width,height);
    setActiveWindow(w2);
    display(imRight);

    Window w3 = openWindow(width,height);
    setActiveWindow(w3);
    display(dispL);

    Window w4 = openWindow(width,height);
    setActiveWindow(w4);
    display(dispR);

    anyClick();

    closeWindow(w1);
    closeWindow(w2);
    closeWindow(w3);
    closeWindow(w4);
}
