#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
//#include <cv.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
void DensMap(Mat dst);

int main( char argc, char *argv[])
{
    Mat img, dst_erode, element;

    // Load image
    if( argc != 2)
    {
     cout <<" Usage: PAIM_prj2_sample ImageToAnalyze" << endl;
     return -1;
    }

    img = imread(argv[1], CV_LOAD_IMAGE_COLOR);// Read the file.

    if(! img.data )// Check for invalid input
    {
     cout <<  "Could not open or find the image" << std::endl ;
     return -1;
    }

    // Show image size in the Console
    double ancho=static_cast<double>(img.size().width)/2100;
    double alto=static_cast<double>(img.size().height)/2100;
    double areaImagen=ancho*alto;

    namedWindow( "Original image", CV_WINDOW_NORMAL );// Create a window for display.
    imshow( "Original image", img );// Show our image inside it.
    cout << "Image Size in pixels: " << img.size() << endl;
    cout << "Image Size in mm: " << "(" << static_cast<double>(img.size().width)/2100 << " x " <<
                static_cast<double>(img.size().height)/2100 << ")" << endl;

    // BGR channels:
    vector<Mat> BGR_planes;
    split( img, BGR_planes );

    // it seems that adding blue and green channel, nuclei are highlighted
    Mat dst, srcBG = BGR_planes[0]+BGR_planes[1];
    adaptiveThreshold( srcBG,  dst,  255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 31, 0);
    //imshow("Binarized image (adaptive)", dst);

    DensMap(dst);

    //Erosion
    int size=4;
    element = getStructuringElement( MORPH_RECT,Size( 2*size, 2*size ),Point( size, size ) );
    erode(dst,dst_erode,element);
    //Dilatacion
    dilate(dst_erode,dst,element);
    medianBlur(dst, dst, 5);

    //Find Countours
    vector<vector<Point> > contours;
    findContours(dst, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    drawContours(dst, contours,-1,0,-1,8,noArray(),255,Point()); //Draw all contours to 0.

    //Working with areas
    double curva;
    double circularidad;
    bool closed=false;
    list<double> Circuls; //Used to count the number of cells
    int cells;

    for (int i=0; i<contours.size();i++)
    {
        curva = arcLength(contours[i],closed); //Calculate the perimeter of the contour
        circularidad = ((contourArea(contours[i], false))*4*(atan(1)*4))/(curva*curva);
        //cout << "Circularidad: "<<circularidad<<endl;
        if(circularidad>0.7 && curva<80)
        {
            Circuls.push_back(circularidad); //Save the contour that meets the requeriment to be a cell
            drawContours(dst, contours,i,255,-1,8,noArray(),255,Point()); //Draw the suppose cell with 255 grayscale
        }
    }

    namedWindow("Final",WINDOW_NORMAL);
    imshow("Final", dst);

    //Showing the info
    cells = Circuls.size();
    cout << "Number of cells: " << cells <<" cells" << endl;
    cout << "Number of cells per mm2: " << cells/areaImagen << endl;

    waitKey(0);
    return 0;
}

void DensMap(Mat dst)
{

    Mat mSource_Gray = dst;
    Mat mBlobHeatMap,mHeatMap, mDist,mBlobDist;

    distanceTransform(mSource_Gray, mDist, CV_DIST_L2, 3);
    normalize(mDist, mDist, 0, 5., cv::NORM_MINMAX);
    mDist.convertTo(mDist,CV_8UC1,255,0);
    //imshow("mDist",mDist);

    //Find Countours
    vector<vector<Point> > contours;
    findContours(mSource_Gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    //Working with areas
    double curva;
    double circularidad;
    bool closed=false;
    list<double> Circuls;
    Mat mBlobMask = Mat::zeros( mSource_Gray.size(), CV_8UC1 );

    for (int i=0; i<contours.size();i++)
    {
        curva = arcLength(contours[i],closed); //Calculate the perimeter of the contour
        circularidad = ((contourArea(contours[i], false))*4*(atan(1)*4))/(curva*curva);
        //cout << "Circularidad: "<<circularidad<<endl;
        if(circularidad>0.1 && curva<80)
        {
            Circuls.push_back(circularidad); //Save the contour that meets the requeriment to be a cell
            drawContours( mBlobMask, contours, (int)i, Scalar(255), -1);
            mDist.copyTo(mBlobDist,mBlobMask);
            applyColorMap(mBlobDist,mBlobHeatMap,COLORMAP_RAINBOW);
            GaussianBlur(mBlobHeatMap,mBlobHeatMap,Size(21,21),0,0);
            mBlobHeatMap.copyTo(mHeatMap,mBlobMask);
        }
    }
    imshow("mHeatMap",mHeatMap);
}
