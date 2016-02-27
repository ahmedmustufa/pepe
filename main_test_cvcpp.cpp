#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>


#include<iostream>
#include<stdio.h>


using namespace cv;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    //create cascade object for face detection
    CascadeClassifier face_cascade;
    //use library
    face_cascade.load("haarcascade_frontalface_alt.xml");

    if (!face_cascade.load("haarcascade_frontalface_alt.xml")) {
        cout << "cant find file";
    }

    //videocapture
    VideoCapture captureDevice;
    captureDevice.open(0);
   // waitKey(1000);

    //setup image capture
    Mat captureframe;
    Mat grayscale;
    int counter = 0;
    //create a window to present the result
    namedWindow("outputCapture", 1);
   // waitKey(1000);
    int previous_counter = 0;
    //create a loop to capture and find faces
    while (true) 
    {
        //capture a new image frame
        captureDevice >> captureframe;
        //waitKey(1000);

        //convert captured image to gray scale and equalize
        cvtColor(captureframe, grayscale, CV_BGR2GRAY);
        equalizeHist(grayscale, grayscale);

        //create a vector to store the face
        vector<Rect>faces;

        //find faces and store them in the vector array
        face_cascade.detectMultiScale(grayscale,faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE, Size(30, 30));

        static int count = 0;
        int current_counter = faces.size();

        if (faces.size() != count) {
            cout << "number of faces detected: " << faces.size() << endl;
            
        }
        //assign number of faces to count
         count = faces.size();
         if (current_counter != previous_counter) {

         }

        //draw a rectangle for all found in the vector array on the original image
        for (int i = 0; i < faces.size(); ++i) {
            Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            Point pt2(faces[i].x, faces[i].y);

            rectangle(captureframe,pt1,pt2,CvScalar(0,255,0,0),1,8,0);
           
        }
       // if (faces.size()== 0) ++counter;

        //print the output 
        imshow("outputCapture", captureframe);
        //cout << counter;

        //pause for 33 sec
        waitKey(33);
    }
    //cout << counter;
}


