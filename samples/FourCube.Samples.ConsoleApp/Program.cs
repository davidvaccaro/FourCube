using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using FourCube;

namespace FourCube.Samples.ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {

            // Create a new FourCube instance
            OCR ocr = OCR.Create(string.Empty, "eng");

            //ocr.Bound = new System.Drawing.Rectangle(0, 0, 640, 240);

            // Process the whole file
            //System.Console.Write(ocr.Process("C:\\test.tif"));

            // Extract all the bits of text
            List<FourCube.Extract> res = ocr.Extract(System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "test.tif"), ExtractType.Word);

            // Sleep for 5 seconds
            System.Threading.Thread.Sleep(60000);

            // Return
            return;

        }
    }
}
