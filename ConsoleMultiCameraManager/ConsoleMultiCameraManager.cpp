// ConsoleMultiCameraManager.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
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

    std::cout << "Hello World!\n";

    CameraManager::InitializeAllCamera();

    string strArr[20];

    CameraManager::GetAllCameraNames(strArr);

    CameraManager::AcquisitionStart(0);

    unsigned int* ptr = (unsigned int*)malloc(5320 * 4600 * 8 * 4); //必須先提供記憶大小
    CameraManager::Grab(0, ptr);
        
    free(ptr);

    CameraManager::AcquisitionStop(0);

    //Mat image_input(4600, 5320, CV_8UC3, ptr); // THIS IS THE INPUT IMAGE, POINTER TO DATA			

    //resize(image_input, image_input, Size(532, 460));

    //imshow("debug", image_input);
    //cv:waitKey(0);

}