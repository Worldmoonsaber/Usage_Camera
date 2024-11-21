// ConsoleMultiCameraManager.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "MultiCameraManager.h"

int main()
{
    std::cout << "Hello World!\n";

    CameraManager::InitializeAllCamera();

    string strArr[20];

    CameraManager::GetAllCameraNames(strArr);
}