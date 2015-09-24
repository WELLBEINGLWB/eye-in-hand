#ifndef Pollini_project_h
#define Pollini_project_h


// #include <boost/thread/thread.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <utility>
#include <list>
#include <string>
//#include <math.h> 
#include <stdio.h>
#include <eigen3/Eigen/Dense>
//ros
//#include <ros/ros.h>


//opencv
// #include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/core/core_c.h>
// #include <opencv2/core/core.hpp>
// #include <opencv2/flann/miniflann.hpp>
// #include <opencv2/imgproc/imgproc_c.h>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/video/video.hpp>
// #include <opencv2/features2d/features2d.hpp>
// #include <opencv2/objdetect/objdetect.hpp>
// #include <opencv2/calib3d/calib3d.hpp>
// #include <opencv2/ml/ml.hpp>
// #include <opencv2/highgui/highgui_c.h>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/contrib/contrib.hpp>




using namespace cv;



class Camera
{
	private:
		// ros::NodeHandle nodeH;
		Mat scene;
		// Mat frame_;
		Mat duplicate_scene;
		Mat objcet_recognition;
		std::string CAMERA_ROBOT;
		//Eigen::Vector2d Data_point;
		double frame_width;
		double frame_height;
		static Point pos_object;
		Point pos_object_real;
		std::vector<cv::Point> CorretObjectPos;
		std::vector<KeyPoint> kp1;
		Mat des1;

	public:
		void ControllCamera();
		// void setMouseCallback();
		static void CallBackFunc(int event, int x, int y, int flags, void* userdata);
		void DuplicateScene(Mat &frame_t);
		void ShapeDetect();
		void GetCenter(std::vector<cv::Point> objects);

		Camera()
		{	

			// pos_object_real = pos_object;
			//ros::param::get("~Data_point", Data_point);
		}




	~Camera(){};

};



void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);



Point Camera::pos_object;
























#endif