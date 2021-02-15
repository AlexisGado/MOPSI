#include "depth.h"

namespace global {
    const float DENOISE_SCALE = 1;
    const float DENOISE_WEIGHT = 3;
    const float SMOOTHING_THRESHOLD1 = 4.;
    const float SMOOTHING_THRESHOLD2 = 2;
    const float PATH_SCALE = 0.6; //0.875 dans l'article
    const float PATH_VALUE = 1;
    const float SIZE_REDUCTION = 5;
    const int ROW_DISPLAYED = 130;
    const float TOL = 0.05;
}


template <typename T>
StereoImages<T>::StereoImages(string nameImL, string nameImR, bool reusing_path0, bool smoothing0)
{
    if (!load(imLeft,srcPath("images")+string("/")+nameImL)) throw "Issue when loading the left image";
    if (!load(imRight,srcPath("images")+string("/")+nameImR)) throw "Issue when loading the right image";

    width = imLeft.width();
    height = imLeft.height();
    ecart = int(width/global::SIZE_REDUCTION);

    dispL = Image<byte>(width,height);
    dispR = Image<byte>(width,height);

    dispMatrixL = Matrix<int>(height, width);
    dispMatrixR = Matrix<int>(height, width);

    smoothing = smoothing0;
    reusing_path = reusing_path0;
}

// Fonction de calcul générique (sous entendu pour < byte >)
template <typename T>
int StereoImages<T>::dif(int row, int colLeft, int colRight) const
{
    return abs(imLeft(colLeft, row)-imRight(colRight, row));
}

// Fonction de calcul pour les images en couleur
template <>
int StereoImages< RGB< byte >>::dif(int row, int colLeft, int colRight) const
{   int diff = abs(imLeft(colLeft, row).r()-imRight(colRight, row).r());
    diff += abs(imLeft(colLeft, row).g()-imRight(colRight, row).g());
    diff += abs(imLeft(colLeft, row).b()-imRight(colRight, row).b());
    return diff;
}

float smoothingFunction(const float & current_difference_value, const float & former_difference_value){
    if (abs(current_difference_value - former_difference_value) > global::SMOOTHING_THRESHOLD1){
        return current_difference_value;
    }
    if (abs(current_difference_value - former_difference_value) > global::SMOOTHING_THRESHOLD2){
        return (current_difference_value + former_difference_value)/2;
    }
    return former_difference_value;
}

template <typename T>
float StereoImages<T>::computeColorInput(const int & row, Matrix<float> & smooth, const int & x, const int & y) const
{   //Difference
    float colorInput = dif(row,y,x);

    //Denoise
    colorInput = global::DENOISE_SCALE * colorInput + global::DENOISE_WEIGHT;

    //Smoothing
    if(smoothing){
        colorInput = smoothingFunction(colorInput, smooth(y, x));
        smooth(y, x) = colorInput;
    }
    return colorInput;
}

void reusingPaths(float & minimum, Matrix<float> & path, const int & x, const int & y){
    minimum -= path(y, x);
    path(y, x) = path(y, x) * global::PATH_SCALE;
}

template <typename T>
void StereoImages<T>::computeDPMatrix(Matrix<float>& dpMat, int row, Matrix<float> & path, Matrix<float> & smooth, bool fast) const
{
    //Note : dpMat is a square matrix width*width

    if (fast){

        for (int x = 0; x<width; x++)
        {
            for (int y = 0; y< width; y++)
            {
                dpMat(y,x) = 1000000 ;
            }
        }
        dpMat(0,0) = 0;

        for (int x = 1; x<width; x++)
        {
            dpMat(0,x) = dpMat(0,x-1) + dif(row,0,x);
        }
        for (int y = 1; y<width; y++)
        {
            dpMat(y,0) = dpMat(y-1,0) + dif(row,y,0);
        }
        for (int x = 1; x<width; x++)
        {
            if (x-ecart-1 > 1){
                int y = x - ecart - 1;
                float up = dpMat(y,x-1);
                float minimum = min(dpMat(y-1,x-1), up);
                if(reusing_path){
                    reusingPaths(minimum, path, x, y);
                }
                dpMat(y,x) = computeColorInput(row, smooth, x, y) + minimum ;
                if(row ==global::ROW_DISPLAYED){
                    //cout << "ICI DP[" << x << ", " << y << "]= " << dpMat(y,x) << endl;
                }
            }
            for (int y = max(1, x-ecart); y< min(width, x+ecart); y++){
                float up = dpMat(y,x-1);
                float left = dpMat(y-1,x);
                float minimum = min(dpMat(y-1,x-1), min(up,left));
                if(reusing_path){
                    reusingPaths(minimum, path, x, y);
                }
                dpMat(y,x) = computeColorInput(row, smooth, x, y) + minimum ;
                /*if(row == global::ROW_DISPLAYED){
                    cout << "up = DP[" << x-1 << ", " << y << "]= " << up << endl;
                    cout << "left = DP[" << x << ", " << y-1 << "]= " << left << endl;
                    cout << "diag = DP[" << x-1 << ", " << y-1 << "]= " << dpMat(y-1,x-1) << endl;
                    cout << "DP[" << x << ", " << y << "]= " << dpMat(y,x) << endl;
                }*/
            }
            if (x + ecart < width){
                int y = x + ecart;
                float left = dpMat(y-1,x);
                float minimum = min(dpMat(y-1,x-1), left);
                if(reusing_path){
                    reusingPaths(minimum, path, x, y);
                }
                dpMat(y,x) = computeColorInput(row, smooth, x, y) + minimum ;
                /*if(row ==global::ROW_DISPLAYED){
                    cout << "left = DP[" << x << ", " << y-1 << "]= " << left << endl;
                    cout << "diag = DP[" << x-1 << ", " << y-1 << "]= " << dpMat(y-1,x-1) << endl;
                    cout << "Là DP[" << x << ", " << y << "]= " << dpMat(y,x) << endl;
                }*/
            }
        }
    }

    else{
        dpMat(0,0) = 0;

        for (int x = 1; x<width; x++)
        {
            dpMat(0,x) = dpMat(0,x-1) + dif(row,0,x);
        }
        for (int y = 1; y<width; y++)
        {
            dpMat(y,0) = dpMat(y-1,0) + dif(row,y,0);
        }
        for (int x = 1; x<width; x++){
            for (int y = 1; y< width; y++){
                float up = dpMat(y,x-1);
                float left = dpMat(y-1,x);
                float minimum = min(dpMat(y-1,x-1), min(up,left));
                if(reusing_path){
                    reusingPaths(minimum, path, x, y);
                }
                dpMat(y,x) = computeColorInput(row, smooth, x, y) + minimum ;
            }
        }
    }
}

template <typename T>
void StereoImages<T>::addRowDisparityMaps(int row, Matrix<float> & path, Matrix<float> & smooth, bool fast)
{
    Matrix<float> dpMat(width,width);
    computeDPMatrix(dpMat, row, path, smooth, fast);

    Image<byte> byte_dpMat(width,width);
    if (row == global::ROW_DISPLAYED){
        pair<int,int> minmaxL = range(dpMat);
        int minL = minmaxL.first;
        int maxL = minmaxL.second;
        for (int x=0; x<width; x++)
        {
            for (int y=0; y<width; y++)
            {
                byte_dpMat(x,y) = byte(255*float((dpMat(y,x)-minL))/float(maxL-minL));
            }
        }
    }
    int x = width-1;
    int y = width-1;

    float up = 0;
    float left = 0;
    float upLeft = 0;
    float minimum = 0;

    dispMatrixL(row, 0) = 0;
    dispMatrixR(row, 0) = 0;

    while (x!=0 || y!=0)
    {
        byte_dpMat(y,x) = 255;
        dispMatrixL(row, y) = x-y;
        dispMatrixR(row, x) = y-x;
        if(reusing_path){
            path(y, x) = (global::PATH_VALUE + path(y, x))/2 ;
        }
        if (y==0) {x--; continue;}
        if (x==0) {y--; continue;}
        if (y>0) up = dpMat(x,y-1);
        if (x>0) left = dpMat(x-1,y);
        if (x>0 && y>0) upLeft = dpMat(x-1,y-1);
        minimum = min(up,min(left,upLeft));
        if (minimum == upLeft) {y--; x--; continue;} // Add a tolerance in the test if dpMat becomes a float matrix
        if (minimum == up) {y--; continue;}
        if (minimum == left) {x--; continue;}
    }

    if (row == global::ROW_DISPLAYED){
        Window w5 = openWindow(width,width, "byte_dpMat");
        setActiveWindow(w5);
        clearWindow();
        display(byte_dpMat);
    }
}

template <typename T>
void StereoImages<T>::computeDisparity(bool fast)
{   clock_t t1 = clock();
    //Matrix<double> invDispMatrixL(height, width);
    //Matrix<double> invDispMatrixR(height, width);
    Matrix<float> path(width, width);
    Matrix<float> smooth(width, width);
    for (int x = 0; x<width; x++)
    {
        for (int y = 0; y<width; y++)
        {
            path(y, x) = 0;
            smooth(y, x) = 0;
        }
    }

    for (int row = 0; row<height; row++)
    {
        addRowDisparityMaps(row, path, smooth, fast);
    }

    pair<int,int> minmaxL = range(dispMatrixL);
    pair<int,int> minmaxR = range(dispMatrixR);

    int minL = minmaxL.first;
    int minR = minmaxR.first;

    int maxL = minmaxL.second;
    int maxR = minmaxR.second;

    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            dispL(x,y) = byte(255*float((dispMatrixL(y,x)-minL))/float(maxL-minL));
            dispR(x,y) = byte(255*float((dispMatrixR(y,x)-minR))/float(maxR-minR));
        }
    }

    /*for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            invDispMatrixL(y,x) = min(30,abs(1000/dispMatrixL(y,x)));
            invDispMatrixR(y,x) = min(30,abs(1000/dispMatrixR(y,x)));
        }
    }


    pair<int,int> minmaxL = range(invDispMatrixL);
    pair<int,int> minmaxR = range(invDispMatrixR);

    int minL = minmaxL.first;
    int minR = minmaxR.first;

    int maxL = minmaxL.second;
    int maxR = minmaxR.second;

    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            //dispL(x,y) = byte(255*float((maxL-invDispMatrixL(y,x)))/float(maxL-minL));
            //dispR(x,y) = byte(255*float((maxR-invDispMatrixR(y,x)))/float(maxR-minR));
            dispL(x,y) = byte(255 - float(invDispMatrixL(y,x)*255)/float(maxL));
            dispR(x,y) = byte(255 - float(invDispMatrixR(y,x)*255)/float(maxR));
        }
    }*/

    clock_t t2 = clock();
    string param;
    if(fast){
        param = "Fast speed";
    }
    else{
        param = "Basic speed";
    }
    if(reusing_path){
        param += ", Reusing paths";
    }
    if(smoothing){
        param += ", Smoothing";
    }
    cout << "Paramètres de calcul : " << param << endl;
    cout << "Temps de calcul : " << (t2-t1) / float(CLOCKS_PER_SEC) << " secondes." << endl;
}

template <typename T>
void StereoImages<T>::displayAll() const
{
    Window w1 = openWindow(width,height, "Left Image");
    setActiveWindow(w1);
    display(imLeft);

    Window w2 = openWindow(width,height, "Right Image");
    setActiveWindow(w2);
    display(imRight);


    string strL = "Left disparity map";
    string strR = "Right disparity map" ;
    if(reusing_path){
        strL += ", Reusing paths";
        strR += ", Reusing paths";
    }
    if(smoothing){
        strL += ", Smoothing";
        strR += ", Smoothing";
    }


    Window w3 = openWindow(width,height, strL);
    setActiveWindow(w3);
    display(dispL);


    Window w4 = openWindow(width,height, strR);
    setActiveWindow(w4);
    display(dispR);


    // DoublePoint3 * points = new DoublePoint3[width*height];
    // for (int x = 0; x < width; x++){
    //     for (int y = 0; y < height; y++){
    //         DoublePoint3 newPoint (x, y, min(150,abs(1000/dispMatrixL(y,x))));
    //         points[x + width*y] = newPoint;
    //     }
    // }

    // Quad * quads = new Quad[(width-1)*(height-1)];
    // for (int x = 0; x < width-1; x++){
    //     for (int y = 0; y < height-1; y++){
    //         Quad quad (x+width*y, x+1+width*y, x+1+width*(y+1), x+width*(y+1));
    //         quads[x + (width-1)*y] = quad;
    //     }
    // }

    // Mesh mesh(points, width*height, 0, 0, quads, (width-1)*(height-1));

    // mesh.setColor(WHITE);

    // Window w5 = openWindow3D(width,height,"3D representation");
    // setActiveWindow(w5);

    // showMesh(mesh);






    // DoublePoint3 pos(width,height/2,0);
    // DoubleVector3 dir(1,0,0);
    // DoubleVector3 up(0,-1,0);
    // float theta = 0;

    // for (int i=0; i<200; i++)
    // {
    //     theta = M_PI*i/200;
    //     DoublePoint3 pos(-sin(theta/2)*width+cos(theta/2)*2*width,height/2,-width*sin(theta));
    //     DoubleVector3 dir(cos(theta),0,-sin(theta));
    //     setCamera(pos,dir,up);
    //     milliSleep(300);
    // }
    
    // DoublePoint3 pos(width/2,height/2,1150);
    // DoubleVector3 dir(0,0,1);
    // DoubleVector3 up(0,-1,0);
    // setCamera(pos,dir,up);

    setActiveWindow(w4);
    click();

    closeWindow(w1);
    closeWindow(w2);
    closeWindow(w3);
    closeWindow(w4);
}

template <typename T>
void StereoImages<T>::computeScore(string namegtL, string namegtR) const
{
    Image<byte> gtL, gtR;
    if (!load(gtL,srcPath("images")+string("/")+namegtL)) throw "Issue when loading the left gt";

    cout<< "Calcul du score " << endl;
    int correctsL = 0;
    double tolPix = global::TOL*width;
    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            if (abs(gtL(y,x)-dispMatrixL(y,x))<tolPix)
            {
                correctsL++;
            }
        }
    }

    cout<< "Pourcentage de pixels corrects : " << double(correctsL*100)/(width*height) << " %" << endl;
}

template class StereoImages<byte>;
template class StereoImages<Color>;
//Indispensable de mettre ça dans le .cpp, car sinon on devrait mettre l'implémentation de la classe template dans le .h
//cf http://annabellecollin.perso.math.cnrs.fr/TPS/TP_4.pdf
