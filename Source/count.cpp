#include <cv.h>   		
#include <highgui.h>
#include <vector>	
#include <iostream>		
using namespace cv; 
using namespace std;

// Find actual pixel size of Kinect camera
// Find out what the depth image is actually measuring

int find(int*& parent, int x);
int unionB(int* parent, int a, int b);
void connect_components(int NUM_ROW,int NUM_COL, int*& diffL, int*& comp, int*& comp_labels, int& num_labels);

int main( int argc, char** argv )
{
	/*
		Image objects.
	*/
  	Mat img, res;	// Initial image and mean shift segmented image.
	
  	/*
		Mean shift filtering parameters
  	*/
	int spatialRad = 6;
  	int colorRad = 13; 
  	int maxPyrLevel = 3;

  	/*
		Misc. Variables
  	*/
	int neighbours[4]; // Four neightbours of the concerned picture

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

		// connect_components(numRows, numCols, diffL, comp, comp_lables, num_labels)
		connect_components(res.rows , res.cols, int*& diffL, int*& comp, int*& comp_labels, int& num_labels);

		return 0;
	} else {
		cout << "\n***** Error *****\nPlease supply image.";
	}

}

inline int find(int*& parent, int x) {
	int r, i;
	r= x;
	while(parent[r]>= 0) {
		r= parent[r];
	}
	i= x;
	while(parent[i] >= 0) {
		x= parent[i];
		parent[i]= r;
		i= x;
	}

	return r;
}

inline int unionB(int* parent, int a, int b) {
	int size, v= b, w= a;
	a= find(parent,a);
	b= find(parent,b);

	if(a == b) return 0;

	size= abs(parent[a])+abs(parent[b]);

	//if(abs(parent[a])>TH || abs(parent[b])>TH || size>TH) return;

	if(abs(parent[a])>abs(parent[b])) {
		parent[b]= a;
		parent[a]= -size;
	} else {
		parent[a]= b;
		parent[b]= -size;
	}

	return 1;
}

/*
	NUM_ROW is the number of rows in the image
	NUM_COL is the number of columns in the image

*/
void connect_components(int NUM_ROW,int NUM_COL, int*& diffL, int*& comp, int*& comp_labels, int& num_labels) {
	// comp= new int[NUM_ROW*NUM_COL];
	//comp_labels= new int[NUM_ROW*NUM_COL];

	/*
		I believe this is just initializing comp to -1,
		which means I should probably just pass in an empty
		array for comp.
	*/
	for(int j= 0; j<NUM_ROW; j++) {			
		for(int i= 0; i<NUM_COL; i++) {
			int k=j*NUM_COL+i; 				
			comp[k]= -1;
		}
	}

	for(int j= 0; j<NUM_ROW; j++) {
		for(int i= 1; i<NUM_COL; i++) {
			int k=j*NUM_COL+i;		 // This is getting the index in the comp array ( I think )
			int kmNC= k-NUM_COL;	 // kmNC = left neighbor?
			int kmNCm1= k-NUM_COL-1; // kmNCm1 = right neighbor?
			int km1= k-1;
			if(i== 1 && j==0)
				cout<<"DIFFL "<<diffL[k]<<" "<<diffL[km1]<<endl;
			if(j>0 && diffL[k] == diffL[kmNC])
				unionB(comp,k,kmNC);
			if(i>0 && j>0 && diffL[k] == diffL[kmNCm1])
				unionB(comp,k,kmNCm1);
			if(i>0 && diffL[k] == diffL[km1])
			unionB(comp,k,km1);
		}
	}
}
