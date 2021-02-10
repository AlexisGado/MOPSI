#include <iostream>
#include <Imagine/Images.h>
#include <Imagine/LinAlg.h>
#include <Imagine/Graphics.h>
#include <math.h>
#include <time.h>

using namespace std;
using namespace Imagine;

template <typename T> class StereoImages
{
    Image<T> imLeft, imRight;
    Image<byte> dispL, dispR;
    int width,height;
    int ecart;
    bool reusing_path, smoothing;
public:
    StereoImages(string imL, string imR, bool reusing_path0 = true, bool smoothing0 = true);
    void computeDPMatrix(Matrix<float>& dpMat, int row, Matrix<float> & path, Matrix<float> & smooth) const;
    int dif(int row, int colLeft, int colRight) const;
    void addRowDisparityMaps(Matrix<int>& dispMapL, Matrix<int>& dispMapR, int row, Matrix<float> & path, Matrix<float> & smooth) const;
    void computeDisparity();
    void displayAll() const;
};
