using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //ConsoleApp.PlayNitrideCameraCoponent.DllEntryTest();
            ConsoleApp.PlayNitrideCameraCoponent.InitializeAllCamera();

            List<string> list = new List<string>();

            string strVal;
            ConsoleApp.PlayNitrideCameraCoponent.GetAllCamera(out list);

            ConsoleApp.PlayNitrideCameraCoponent.SetCameraParam(0, "Name", "Value");
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "TestNode", out strVal);
            //ConsoleApp.PlayNitrideCameraCoponent.SetCameraParam(0, "testName", "testValue");

            ConsoleApp.PlayNitrideCameraCoponent.AcquisitionStart(0);


            IntPtr ImgPtr=ConsoleApp.PlayNitrideCameraCoponent.Grab(0);


            string strW,strH,strCh;
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Width",out strW);
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Height", out strH);
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Channels", out strCh);

            int width = Convert.ToInt32(strW);   // 圖片寬度
            int height = Convert.ToInt32(strH);  // 圖片高度
            int stride = width * 3; // 每行的字節數（RGB 每像素 3 字節）
            int channels=Convert.ToInt32(strCh);



            // 獲取數據指針

            // 創建 Bitmap
            //Bitmap bitmap = new Bitmap(width,height,stride, PixelFormat.Format24bppRgb, ImgPtr);


            //Mat img= Mat.f("C:\\Image\\4X4\\1836\\183601.bmp");

            //// 創建 Bitmap 並將像素數據封裝進去
            //Bitmap bitmap = new Bitmap(width, height, stride, PixelFormat.Format24bppRgb, ImgPtr);

            //// 使用 Bitmap
            //Console.WriteLine("Bitmap Size: " + bitmap.Width + "x" + bitmap.Height);
            //Mat img =Mat.FromPixelData(width, height, MatType.CV_8UC4, ImgPtr);
            //Bitmap bitmap = Bitmap.FromHbitmap(ImgPtr);
            

            ConsoleApp.PlayNitrideCameraCoponent.AcquisitionStop(0);


            ConsoleApp.PlayNitrideCameraCoponent.CloseAllCamera();

            Console.ReadLine();

        }
    }
}
