#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include<conio.h>
#include <Windows.h>
#include<vector>



using namespace cv;
using namespace std;

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

vector<string> getVideoFIle(string folder)
{
	vector <string> names;
	char search_path[200];
	sprintf(search_path, "%s/*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	wchar_t * tr = convertCharArrayToLPCWSTR(search_path);
	HANDLE hFind = FindFirstFile(tr, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				//names.push_back(fd.cFileName);
				cout << fd.cFileName;
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}



static void FaceDetection(string& videoFile,string& classifier_address, vector<Rect> &faces)
{

}

int main()
{
	RNG rng(12345);
	char ch;
	Mat frame,original_image;
	int countFrame;
	double t;
	CascadeClassifier face_cascade;

	//if (!face_cascade.load("haarcascade_frontalface_alt_tree.xml"))
	//if (!face_cascade.load("haarcascade_frontalface_default.xml"))
	//if (!face_cascade.load("haarcascade_frontalface_alt.xml"))
	if (!face_cascade.load("lbpcascade_frontalface.xml"))
	{
		cout << "Loading failed Check that files (Harr Cascade)are in same directory of not\n";
		cin >> ch;
		return -4;
	}

	//CvCapture *cap = cvCaptureFromAVI("happyfaces.mp4");
	VideoCapture cap("happyfaces.mp4");
	if (!cap.isOpened())
	{
		cout << "Capture Error FUCK OFF\n";
		vector<string> s = getVideoFIle("E:/newCheck");
		cin >> ch;
		return -4;
	}
	else
	{
		int cuit = 0;
		countFrame = 0;
		t = (double)getTickCount();
		while (true)
		{
			cap >> frame;

			if (!frame.empty())
			{
				countFrame++;
				if (countFrame == 100)
				{
					t = ((double)getTickCount() - t) / getTickFrequency();
					double fps = 100 / t;
					cout << "\nFPS = " << fps;
					countFrame = 0;
					t = (double)getTickCount();
					//break;
				}
				std::vector<Rect> faces;
				Mat frame_gray;
				cvtColor(frame, frame_gray, CV_BGR2GRAY);
				equalizeHist(frame_gray, frame_gray);

				face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
				//cout << "\nHHHHHH\t56";
				for (size_t i = 0;i < faces.size();i++)
				{

					Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
					int radius = cvRound((faces[i].width + faces[i].height)*0.25);
					//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					circle(frame, center, radius, Scalar(0, 255, 0), 4, 8, 0);
					Rect roi = faces[i];
					// Cut it out of the original image
					Mat face = frame(roi).clone();
					// Store the image
					imwrite("E:\*.jpg", faces);
					imshow("YO_BITCH", frame);
				}

				int c = waitKey(1);
				if (c == 27)
					break;
			}

		}
	}
	cout << "You are ready to go";
	cin >> ch;
	return 0;
}
