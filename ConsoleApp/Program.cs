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
            ConsoleApp.PlayNitrideCameraCoponent.AcquisitionStart(0);


            IntPtr ImgPtr=ConsoleApp.PlayNitrideCameraCoponent.Grab(0);

            string strW,strH,strCh;
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Width",out strW);
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Height", out strH);
            ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, "Channels", out strCh);

            int width = Convert.ToInt32(strW);   // 圖片寬度
            int height = Convert.ToInt32(strH);  // 圖片高度
            int stride = width * 3; // 每行的字節數（RGB 每像素 3 字節）
            int channels=4;

            unsafe
            { //存檔測試
                Bitmap bitmap = debug.CreateBitmapFromPointer(ImgPtr, width, height);
            }

            ConsoleApp.PlayNitrideCameraCoponent.AcquisitionStop(0);
            
            
            ConsoleApp.PlayNitrideCameraCoponent.CloseAllCamera();
        }
    }


    class debug
    {
        public static unsafe Bitmap CreateBitmapFromPointer(IntPtr ptr, int width, int height)
        {
            // 建立 Bitmap
            Bitmap bitmap = new Bitmap(width, height, PixelFormat.Format32bppRgb);

            // 鎖定 Bitmap 的像素資料
            BitmapData bitmapData = bitmap.LockBits(
                new Rectangle(0, 0, width, height),
                ImageLockMode.WriteOnly,
                PixelFormat.Format32bppRgb);

            // 將指標資料複製到 Bitmap
            Buffer.MemoryCopy((void*)ptr, (void*)bitmapData.Scan0, width * height * 4, width * height * 4);

            // 解鎖 Bitmap
            bitmap.UnlockBits(bitmapData);

            return bitmap;
        }
    }
}
