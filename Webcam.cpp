#include "Webcam.h"


void Webcam::FaceRecog()
{
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	if (!face_cascade.load("C:/Open_CV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"))
	{
		cerr << "Error loading face cascade\n" << endl;
	};
	if (!eyes_cascade.load("C:/Open_CV/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"))
	{
		cerr << "Error loading eyes cascade\n" << endl;
	};

	VideoCapture capture;
	//reading the video stream
	capture.open(camera_num);
	if (!capture.isOpened())
	{
		cerr << "Error opening video capture\n" << endl;
	}

	Mat frame;
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			cerr << "No captured frame\n" << endl;
			break;
		};

		detectAndDisplay(frame, face_cascade, eyes_cascade);
		if (waitKey(5) == 'q')
		{
			break;
		};
	}
}


Webcam::Webcam(int camera)
{
	this->camera_num = camera;
}

Mat Webcam::takePicture()
{
	VideoCapture camera(camera_num);
	if (!camera.isOpened())
	{
		std::cerr << "ERROR: Could not open camera" << std::endl;
	}

	Mat frame;
	camera >> frame;
	if (frame.empty())
	{
		std::cerr << "ERROR: Frame is empty." << std::endl;

	}
	return frame;

}
Mat Webcam::savePicture(String fileName)
{
	Mat frame = takePicture();
	imwrite(fileName, frame);
	return frame;
}

void Webcam::detectAndDisplay(Mat frame, CascadeClassifier face_cascade, CascadeClassifier eyes_cascade)
{
	Mat frame_gray, smallImg;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	resize(frame_gray, smallImg, Size(), .5, .5, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	std::vector<Rect> faces;
	face_cascade.detectMultiScale(smallImg, faces);
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point topLeft(faces[i].x * 2, faces[i].y * 2);
		Point topRight((faces[i].x + faces[i].width) * 2, (faces[i].y + faces[i].height) * 2);
		rectangle(frame, topLeft, topRight, Scalar(0, 0, 255), 4);
		Mat faceROI = smallImg(faces[i]);

		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);
		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center((faces[i].x + eyes[j].x + eyes[j].width / 2) * 2, (faces[i].y + eyes[j].y + eyes[j].height / 2) * 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25 * 2);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);
		}
	}
	//-- Show what you got
	imshow("Capture - Face detection", frame);
}