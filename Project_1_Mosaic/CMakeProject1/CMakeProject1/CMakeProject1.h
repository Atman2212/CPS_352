// CMakeProject1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

// OpenCV and standard libraries
using namespace cv;
using namespace std;

// Function declarations
void blurBlock(Point p1, Point p2);
void onMyMouse(int event, int x, int y, int flag, void* data);
void handleKeyboardInput(char c);