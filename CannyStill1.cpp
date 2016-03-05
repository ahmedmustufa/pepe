#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<stdio.h>
#include<curl/curl.h>
#include<string>
#include<iostream>
#include<cstring>

using namespace cv;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://dweet.io/dweet/for/sparty_party?hello=world");
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	}

	//create cascade object for face detection
	CascadeClassifier face_cascade;

	//CascadeClassifier body_cascade;
	//use library
	face_cascade.load("haarcascade_frontalface_alt.xml");

	if (!face_cascade.load("haarcascade_frontalface_alt.xml")) {
		cout << "cant find file";
	}

	//videocapture


	// http://35.13.34.75:8080/video

	VideoCapture captureDevice;
	captureDevice.open(0);

	//setup image capture
	Mat captureframe;
	Mat grayscale;
	int counter = 0;
	//create a window to present the result
	namedWindow("outputCapture", 0);
	int previous_counter = 0;
	//create a loop to capture and find faces

	while (true)
	{
		//capture a new image frame
		captureDevice >> captureframe;

		//convert captured image to gray scale and equalize
		cvtColor(captureframe, grayscale, CV_BGR2GRAY);
		equalizeHist(grayscale, grayscale);

		//create a vector to store the face
		vector<Rect>faces;

		//find faces and store them in the vector array
		face_cascade.detectMultiScale(grayscale, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		static int peoplecounter = 0;
		static int count = 0;
		int current_counter = faces.size();

		if (current_counter > count) {
			peoplecounter += (current_counter - count);
			cout << endl << "Total count: " << peoplecounter << endl << endl;
			string occupants = "occupants=" + to_string(peoplecounter);
			static const char *postthis = occupants.c_str();
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);

			/* Perform the request, res will get the return code */
			res = curl_easy_perform(curl);
			//cout << res << endl;
			/* Check for errors
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
					*/
			/* always cleanup */
			//curl_easy_cleanup(curl);
		}

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

			rectangle(captureframe, pt1, pt2, CvScalar(0, 255, 0, 0), 1, 8, 0);

			//cout << i << "| " << "x: " << faces[i].x << ", y: " << faces[i].y << endl;

		}


		//print the output video
		imshow("outputCapture", captureframe);

		//pause for 0.010 sec
		waitKey(10);
	}

}