#include <stdio.h>
#include <iostream>
//Include OpenCV header files to recognize "Mat"
#include <opencv2/core/core.hpp> //Define the most important class/structure
#include <opencv2/highgui/highgui.hpp> //provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> //provide image processing functions
using namespace cv;
using namespace std; //cout or cin for c++
int main(int argc, char** argv)
{
	Mat img; //Mat is the most important class in opencv
	//"matrix" is considered as an image holder
	img = imread("scene.jpg"); //load image file into RAM and store it as "img"
		while (1)
		{
			imshow("My Window", img); //OpenCV create a window called "My Window"
			// display the img within the "My Window"
			img *= 0.99;
			//Using keyboard to stop the while loop
			char c = waitKey(100);
			if (c == 27) //ASCII code for ESc key
			{
				break;
			}
			if (c == 's' || c == 'S') //if s key is hit
				break;
		}
	return 1;
}
