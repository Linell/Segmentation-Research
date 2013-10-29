#include <cv.h>   		
#include <highgui.h>	
#include <iostream>		
using namespace cv; 

// Sets the camera index.
#define CAMERA_INDEX -1


int main( int argc, char** argv )
{
  	Mat img, res;	    // image objects
  	VideoCapture cap; // capture object
	
  	const string windowName = "Mean Shift Segmentation"; // window name

  	bool keepProcessing = true;		// loop control flag
  	int  key;						// user input	
  	int  EVENT_LOOP_DELAY = 40;		// delay for GUI window
                                	// 40 ms equates to 1000ms/25fps = 40ms per frame	
	
  	int spatialRad = 10;			// mean shift filtering parameters
  	int colorRad = 20; 
  	int maxPyrLevel = 5;

  	int level = 10;
  	int threshold1 = 10;
  	int threshold2 = 20;

  	// if command line arguments are provided try to read image/video_name
  	// otherwise default to capture from attached H/W camera 

    if( 
		( argc == 2 && (!(img = imread( argv[1], CV_LOAD_IMAGE_COLOR)).empty()))||
		( argc == 2 && (cap.open(argv[1]) == true )) || 
		( argc != 2 && (cap.open(CAMERA_INDEX) == true))
	  )
    {
    	// create window object (use flag=0 to allow resize, 1 to auto fix size)

    	namedWindow(windowName, 1);		

		createTrackbar( "spatialRad", windowName, &spatialRad, 80);
    	createTrackbar( "colorRad", windowName, &colorRad, 60);
    	createTrackbar( "maxPyrLevel", windowName, &maxPyrLevel, 5);

		
		// start main loop	
		
		while (keepProcessing) {
		
			// if capture object in use (i.e. video/camera)
			// get image from capture object
			
			if (cap.isOpened()) {
			  
				cap >> img;
				if(img.empty()){
					if (argc == 2){				  
						std::cerr << "The end of the video file has been reached." << std::endl;
					} else {
						std::cerr << "ERROR: cannot get next frame from the camera." << std::endl;
					}
					exit(0);
			 	}
			  
		  	} else {
			  
				// if not a capture object set event delay to zero so it waits
				// indefinitely (as single image file, no need to loop)
			  
				EVENT_LOOP_DELAY = 0;
			}			  
		
			// pyrMeanShiftFiltering(src, dst, sp, sr, max_level=1, termcrit=(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 5, 1))
			// src(CvArr) - the source 8-bit, 3-channel image
			// dst(CvArr) - the destination image of the same size and format as the source
			// sp(float)  - the spatial window radus
			// sr(float)  - the color window radius
			// max_level(int) - maximum level of the pyramid for the segmentation
			// termcrit(CvTermCriteria) - termination criteria: when to stop meanshift iterations
			pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel );
			
			// display image in window		
			imshow(windowName, res);
			  
			// start event processing loop (very important,in fact essential for GUI)
		    // 40 ms roughly equates to 1000ms/25fps = 4ms per frame
			  
			key = waitKey(EVENT_LOOP_DELAY);

			if (key == 'n') {
				std::cout << "\n********************";
				std::cout << "\nspatialRad : " << spatialRad;
				std::cout << "\ncolorRad   : " << colorRad;
				std::cout << "\nmaxPyrLevel: " << maxPyrLevel;

				imshow(windowName, res);
				pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel );
			}

			if (key == 's') {
				EVENT_LOOP_DELAY = 0;
			}

			if (key == 'x') {
				
		   		// if user presses "x" then exit
				
				std::cout << "Keyboard exit requested, now exiting." << std::endl;
		   		keepProcessing = false;
			} 
	  }
     
	  // the camera will be deinitialized automatically in VideoCapture destructor
      // all OK : main returns 0
      return 0;
    }

    // not OK : main returns -1

    return -1;
}
