#include <cv.h>   		
#include <highgui.h>
#include <vector>	
#include <iostream>		
using namespace cv; 
using namespace std;

void FindBlobs(const Mat &binary, vector <vector<Point2i> > &blobs);

int main( int argc, char** argv )
{
	/*
		Image objects.
	*/
  	Mat img, res;	// Initial image and mean shift segmented image.
  	Mat output; 	// Output of blob detection. 
	
  	/*
		Mean shift filtering parameters
  	*/
	int spatialRad = 6;
  	int colorRad = 13; 
  	int maxPyrLevel = 3;

  	/*
		Named Windows
  	*/
	namedWindow("binary");
	namedWindow("labelled");

    if( argc == 2 && (!(img = imread( argv[1], CV_LOAD_IMAGE_COLOR)).empty()) )
    {
	  
		// pyrMeanShiftFiltering(src, dst, sp, sr, max_level=1, termcrit=(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 5, 1))
		// src(CvArr) - the source 8-bit, 3-channel image
		// dst(CvArr) - the destination image of the same size and format as the source
		// sp(float)  - the spatial window radus
		// sr(float)  - the color window radius
		// max_level(int) - maximum level of the pyramid for the segmentation
		// termcrit(CvTermCriteria) - termination criteria: when to stop meanshift iterations
		pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel );

		/*
			Blob detection from nghiaho.com/uploads/code/opencv_connected_component/blob.cpp
		*/
		output = Mat::zeros(res.size(), CV_8UC3);
		vector <vector<Point2i > > blobs;

		FindBlobs(res, blobs);

		// Randomly color the blobs
		for (size_t i = 0; i < blobs.size(); i++) {
			unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
			unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
			unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));

			for (size_t j = 0; j < blobs[i].size(); j++) {
				int x = blobs[i][j].x;
				int y = blobs[i][j].y;

				output.at<Vec3b>(y, x)[0] = b;
				output.at<Vec3b>(y, x)[1] = g;
				output.at<Vec3b>(y, x)[2] = r;
			}
		}
		imshow("segmented", res);
		imshow("labelled", output);
		waitKey(0);

		return 0;
	} else {
		cout << "\n***** Error *****\nPlease supply image.";
	}

}

void FindBlobs (const Mat &img, vector<vector<Point2i> > &blobs) {
	blobs.clear();

	/*
		Fill the label_image with the blobs
		0 - background
		1 - unlabelled foreground
		2+ - labelled forground
	*/
	//Mat label_image;
	//img.convertTo(label_image, CV_32SC1);

	int label_count = 2; // Starts at two because 0 and 1 are already used.

	for (int y = 0; y < img.rows; y++) {
		int *row = (int*)img.ptr(y);
		for (int x = 0; x < img.cols; x++) {
			// if ( row[x] != 1 ) {
			// 	cout << "row[x] is: " << row[x] << endl;
			// 	continue;
			// }
			Rect rect;
			floodFill(img, Point(x, y), label_count, &rect, 0, 0, 4);

			vector <Point2i> blob;

			for (int i = rect.y; i < (rect.y+rect.height); i++) {
				int *row2 = (int*)img.ptr(i);
				for (int j = rect.x; j < (rect.x + rect.width); j++) {
					// if (row2[j] != label_count) {
					// 	cout << "row2[j] is: " << row2[j] << endl;
					// 	continue;
					// }
					blob.push_back(Point2i(j, i));
				}
			}
			blobs.push_back(blob);

			label_count++;
		}
	}
	cout << blobs.size();
}