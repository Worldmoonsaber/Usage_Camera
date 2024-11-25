﻿// ConsoleMultiCameraManager.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp> //mophorlogical operation
#include<opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include "MultiCameraManager.h"

using namespace cv;


int main()
{


    CameraManager::InitializeAllCamera();

    string strArr[20];

    CameraManager::GetAllCameraNames(strArr);

    
    CameraManager::AcquisitionStart(0);



    string strVal;
    CameraManager::GetCameraParam(0,"PixelFormat", strVal);

    int channels = 0;

    if (strVal == "mono8")
        channels = 1;
    else
        channels = 4;

    CameraManager::GetCameraParam(0, "Width", strVal);
    int Width = atoi(strVal.c_str());

    CameraManager::GetCameraParam(0, "Height", strVal);
    int Height = atoi(strVal.c_str());

    unsigned int* ptr = (unsigned int*)malloc(Width * Height * 8 * channels); //必須先提供記憶大小
    CameraManager::Grab(0, ptr);
        
    Mat image_input(4600, 5320, CV_8UC3, ptr); // THIS IS THE INPUT IMAGE, POINTER TO DATA			

    //resize(image_input, image_input, Size(532, 460));
    ////imwrite("E://debug.bmp", image_input);
    //imshow("debug", image_input);
    //cv:waitKey(0);
    //free(ptr);

    CameraManager::AcquisitionStop(0);

}