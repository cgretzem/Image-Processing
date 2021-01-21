#include "opencv2/opencv.hpp"
#include "iostream"


using namespace cv;
using namespace std;
class Webcam
{
private:
	int camera_num;
	void detectAndDisplay(Mat frame, CascadeClassifier face_cascade, CascadeClassifier eyes_cascade);
public:
	Webcam(int camera = 0);
	Mat takePicture();
	Mat savePicture(String fileName);
	void FaceRecog();
};

