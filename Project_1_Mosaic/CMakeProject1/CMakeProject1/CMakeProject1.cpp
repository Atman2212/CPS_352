#include "CMakeProject1.h"
Mat image; //Mat is the most important class in opencv
//"matrix" is considered as an image holder
Mat image_org; //This image is holding the original values
Point pt = Point(-1, -1); //Represent previous point
bool drawing = false; //indicator: the mouse is dragging
int blur_degree = 5; //degree of blur
vector<Rect> blurRegions;  // Stores all selected blur regions
//Dynamic array to store image data
unsigned char** arr2D; //Alternatively, you can use 2D array to operate the image
void blurBlock(Point p1, Point p2) {
	// Ensure p1 is the top-left and p2 is the bottom-right
	int min_x = min(p1.x, p2.x);
	int min_y = min(p1.y, p2.y);
	int max_x = max(p1.x, p2.x);
	int max_y = max(p1.y, p2.y);

	// Loop through the selected region, moving block by block
	for (int y = min_y; y < max_y; y += blur_degree) {
		for (int x = min_x; x < max_x; x += blur_degree) {
			int sum_r = 0, sum_g = 0, sum_b = 0, count = 0;

			int block_h = min(blur_degree, max_y - y);  // Adjust height for edge cases
			int block_w = min(blur_degree, max_x - x);  // Adjust width for edge cases

			// Compute the average color for the current block
			for (int dy = 0; dy < block_h; dy++) {
				for (int dx = 0; dx < block_w; dx++) {
					sum_b += arr2D[y + dy][(x + dx) * 3 + 0];
					sum_g += arr2D[y + dy][(x + dx) * 3 + 1];
					sum_r += arr2D[y + dy][(x + dx) * 3 + 2];
					count++;
				}
			}

			int avg_b = sum_b / count;
			int avg_g = sum_g / count;
			int avg_r = sum_r / count;

			// Apply the mosaic effect to each pixel in the block
			for (int dy = 0; dy < block_h; dy++) {
				for (int dx = 0; dx < block_w; dx++) {
					arr2D[y + dy][(x + dx) * 3 + 0] = avg_b;
					arr2D[y + dy][(x + dx) * 3 + 1] = avg_g;
					arr2D[y + dy][(x + dx) * 3 + 2] = avg_r;
				}
			}
		}
	}

	// Copy the modified arr2D values back into image
	for (int y = min_y; y < max_y; y++) {
		memcpy(image.data + y * image.cols * 3, arr2D[y], image.cols * 3);
	}

	// Redraw the rectangle outline after applying the blur
	rectangle(image, p1, p2, Scalar(0, 255, 255), 3);
}
void onMyMouse(int event, int x, int y, int flag, void* data) {
	if (event == EVENT_LBUTTONDOWN) {
		pt = Point(x, y);
		drawing = true;
	}
	else if (event == EVENT_MOUSEMOVE && drawing) {
		image_org.copyTo(image);  // Reset image to original before redrawing everything

		// Reapply all previous blurs
		for (Rect region : blurRegions) {
			blurBlock(region.tl(), region.br());
		}

		// Redraw all previous rectangles
		for (Rect region : blurRegions) {
			rectangle(image, region.tl(), region.br(), Scalar(0, 255, 255), 3);
		}

		// Draw the current selection dynamically
		rectangle(image, pt, Point(x, y), Scalar(0, 255, 255), 3);
	}
	else if (event == EVENT_LBUTTONUP) {
		drawing = false;

		// Store the new selection
		Rect selectedRegion(pt, Point(x, y));
		blurRegions.push_back(selectedRegion);

		// Reset image to original before applying blur and rectangles
		image_org.copyTo(image);

		// Apply blur for all stored regions
		for (Rect region : blurRegions) {
			blurBlock(region.tl(), region.br());
		}

		// redraw all rectangles so they appear clearly
		for (Rect region : blurRegions) {
			rectangle(image, region.tl(), region.br(), Scalar(0, 255, 255), 3);
		}

		imshow("My Window", image);  // Refresh window with correctly drawn rectangles
	}
}
void handleKeyboardInput(char c) {
	if (c == 'i' || c == 'I') { // Increase blur level
		if (blur_degree < 5) {
			blur_degree++;
		}
		else {
			blur_degree += 5;
		}
		printf("Increased blur degree: %d\n", blur_degree);
	}
	else if (c == 'd' || c == 'D') { // Decrease blur level
		if (blur_degree > 1) {
			if (blur_degree <= 5) {
				blur_degree--;
			}
			else {
				blur_degree -= 5;
			}
		}
		else {
			printf("Blur degree cannot be less than 1.\n");
		}
		printf("Decreased blur degree: %d\n", blur_degree);
	}
	else if (c == 'r' || c == 'R') { // Reset image
		printf("Reset image to original.\n");

		// Restore original image
		image_org.copyTo(image);

		// Clear all stored blur regions
		blurRegions.clear();

		// Reset the arr2D array with fresh pixel values from image_org
		for (int y = 0; y < image.rows; y++) {
			memcpy(arr2D[y], image_org.data + y * image.cols * 3, image.cols * 3);
		}

		// Refresh window to show fully reset image
		imshow("My Window", image);
		return; // Ensure we don't reapply blur after reset
	}
	else if (c == 's' || c == 'S') { // Save image
		imwrite("blurred_output.jpg", image);
		printf("Image saved as blurred_output.jpg\n");
	}

	// Reset to original image before applying updated blur effect
	image_org.copyTo(image);

	// Reset arr2D to avoid applying blur on an already blurred version
	for (int y = 0; y < image.rows; y++) {
		memcpy(arr2D[y], image_org.data + y * image.cols * 3, image.cols * 3);
	}

	// Reapply blur using the NEW blur degree
	for (Rect region : blurRegions) {
		blurBlock(region.tl(), region.br());
	}

	// Reapply rectangle outlines after blur is applied
	for (Rect region : blurRegions) {
		rectangle(image, region.tl(), region.br(), Scalar(0, 255, 255), 3);
	}

	// Refresh the window with the correct blur effect
	imshow("My Window", image);
}


int main(int argc, char** argv)
{
	image = imread("scene.jpg"); //load image file into RAM and store it as "img" variable
	image.copyTo(image_org); //In the beginning, image_original is identical as img2
	printf("Image size: (%d, %d)\n", image.rows, image.cols);
	/** 2D array creation and assign values from image */
	arr2D = new unsigned char* [image.rows]; //Allocate rows or 1st dimension
	for (int y = 0; y < image.rows; y++)
	{
		arr2D[y] = new unsigned char[image.cols * 3]; //allocate space in RAM cols or 2nd dimension
		memcpy(arr2D[y], image.data + y * image.cols * 3, image.cols * 3);
	}
	namedWindow("My Window"); //Create a window called "My Window"
	// This line is optional if you just display an image in the window
	// But if you need the window receive mouse event, you have to declare this window first
	setMouseCallback("My Window", onMyMouse); //register the callback function
	while (1)
	{
		imshow("My Window", image); //OpenCV create a window called "My Window"
		// display the img within the "My Window"
		//Using keyboard to stop the while loop
		char c = waitKey(100); 
		if (c == 27) //ASCII code for ESC key
		{
			break;
		}
		if (c != -1) { // If a key is pressed
			handleKeyboardInput(c);
		}
	}
	return 1;
}
