#include <highgui.h>
#include </opt/local/include/opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat res;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

// You can play with these values through segment.cpp
int spatialRad = 18;
int colorRad = 9; 
int maxPyrLevel = 3;

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );

  pyrMeanShiftFiltering( src, res, spatialRad, colorRad, maxPyrLevel );

  /// Convert image to gray and blur it
  cvtColor( res, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );
  resizeWindow( source_window, 500, 500 );
  /// Create Window
  char* seg_window = "Segmented";
  namedWindow( seg_window, CV_WINDOW_AUTOSIZE );
  imshow( seg_window, res );
  resizeWindow( seg_window, 500, 500 );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ ) {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
  }
  cout << "The image contains " << contours.size() << " contours. Thres: " << thresh << ".\n";
  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  resizeWindow( "Contours", 500, 500 );
}