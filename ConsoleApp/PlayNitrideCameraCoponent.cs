using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace ConsoleApp
{
    public class PlayNitrideCameraCoponent
    {

        // C:\\Git\\Usage_Camera\\ArenaConsole\\x64\\Release\\PlayNitrideCameraComponet.dll
        const string dllPath = "C:\\Git\\Usage_Camera\\ArenaConsole\\x64\\Release\\PlayNitrideCameraComponet.dll";  // 這裡的 DLL 應該是已經編譯好的 DLL 路徑
        //dll\\PlayNitrideCameraComponet.dll

        #region Public

        [DllImport(dllPath, EntryPoint = "CSharp_InitializeAllCamera", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeAllCamera();

        [DllImport(dllPath, EntryPoint = "CSharp_CloseAllCamera", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CloseAllCamera();

        [DllImport(dllPath, EntryPoint = "CSharp_AcquisitionStart", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AcquisitionStart(int cameraId);

        [DllImport(dllPath, EntryPoint = "CSharp_AcquisitionStop", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AcquisitionStop(int cameraId);

        public static void GetAllCamera(out List<string> lstCamera)
        {
            lstCamera = new List<string>();

            int length = 20;
            IntPtr[] arrayPtrs = new IntPtr[length];

            // 調用 C++ 填充字串指針
            _GetAllCamera(arrayPtrs);

            // 解析字串
            for (int i = 0; i < length; i++)
            {
                if (arrayPtrs[i] != IntPtr.Zero)
                {
                    string str = Marshal.PtrToStringAnsi(arrayPtrs[i]);
                    lstCamera.Add(str);
                    FreeIntptrMemory(arrayPtrs[i]);
                    Console.WriteLine("camera : "+str);
                }
            }

        }


        [DllImport(dllPath, EntryPoint = "CSharp_SetCameraParam", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCameraParam(int cameraId, string NodeName, string Value);


        public static void GetCameraParam(int cameraId, string NodeName, out string Value)
        {
            Value = "";
            IntPtr ptr = ConsoleApp.PlayNitrideCameraCoponent.GetCameraParam(0, NodeName);

            if (ptr != IntPtr.Zero)
            {
                Value = Marshal.PtrToStringAnsi(ptr);
                FreeIntptrMemory(ptr);
                Console.WriteLine("C# StrVal : " + Value);

            }

        }


        #endregion

        #region 已確認 內容 功能測試 OK

        [DllImport(dllPath, EntryPoint = "CSharp_GetAllCamera", CallingConvention = CallingConvention.Cdecl)]
        private static extern void _GetAllCamera([Out] IntPtr[] array);

        [DllImport(dllPath, EntryPoint = "CSharp_GetCameraParam", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetCameraParam(int cameraId, string NodeName);

        [DllImport(dllPath, EntryPoint = "CSharp_FreeIntptrMemory", CallingConvention = CallingConvention.Cdecl)]
        private static extern void FreeIntptrMemory(IntPtr str);

        [DllImport(dllPath, EntryPoint = "CSharp_Grab", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Grab(int cameraId);


        [DllImport(dllPath, EntryPoint = "CSharp_GetErrorLog", CallingConvention = CallingConvention.Cdecl)]
        private static extern void _GetAllLog([Out] IntPtr[] array);

        public static void GetAllLog(out List<string> lstLog)
        {
            lstLog = new List<string>();

            int length = 100;
            IntPtr[] arrayPtrs = new IntPtr[length];

            // 調用 C++ 填充字串指針
            _GetAllLog(arrayPtrs);

            // 解析字串
            for (int i = 0; i < length; i++)
            {
                if (arrayPtrs[i] != IntPtr.Zero)
                {
                    string str = Marshal.PtrToStringAnsi(arrayPtrs[i]);
                    lstLog.Add(str);
                    FreeIntptrMemory(arrayPtrs[i]);
                }
            }

        }

        #endregion


        #region 待完善





        #endregion
    }
}



