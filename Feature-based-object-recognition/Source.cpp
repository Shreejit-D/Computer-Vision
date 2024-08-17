/*
@Creator - Shreejit Gajanan Deshmukh
@PRCV assignment 3 - Real-time object recognition
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <fstream>
#include<Functions.h>

/********************************************************************
This code does real time object recognition using shape features.	*
For the object to be detected the image must be dark and background	*
should be white. The object is recognized using image moments which	*
are normalized in scale, rotation, and translation.					*
********************************************************************/

int main(int argc, char* argv[]) {
	cv::Mat image, blur_image, gray_im, bi_image_noisy, bi_image_no_noise, out_image;
	std::cout << "Please input the mode of recognition :- image 'i' or video 'v'" << std::endl;
	char mode;
	std::cin >> mode;
	
	if (mode == 'i') {
		char image_loc[50];
		std::cout << "Specify the image with its location for object detection/feature update from database" << std::endl;
		std::cin >> image_loc;
		image = cv::imread(image_loc);

		//blurring image for better thresholding
		blur5x5(image, blur_image);

		//converting to grayscale image
		cv::cvtColor(blur_image, gray_im, cv::COLOR_BGR2GRAY);

		//thresholding image
		bi_image_noisy = threshold(gray_im);

		//cleaning the image using opencv_func
		denoised(bi_image_noisy, bi_image_no_noise);

		//segmentation using opencv_func
		cv::Mat stats, label, centroid;
		cv::Mat inv;
		cv::bitwise_not(bi_image_noisy, inv);
		int ncomps = cv::connectedComponentsWithStats(inv, label, stats, centroid, 8, CV_16U);

		//check
		//std::cout << "The total components in the image are " << ncomps;

		//coloring different regions
		//color_segments(label, stats, ncomps);

		//Oriented bounding box and moments
		//display_box(image, label, stats, ncomps);
		//cv::waitKey();
		
		//find the features of in the image
		std::vector<float> features = get_features(bi_image_no_noise, ncomps, label, stats);
		
		std::cout << "Specify if you would like to recognise - 'r' an object in image"
			<< " or if you want to store features - 's' in the database" << std::endl;
		char key;
		std::cin >> key;
		
		if (key == 's') {
			//storing features in a csv for later use
			update_database(features);

			//loading the features database
			std::vector<std::vector<std::string>>features_db = load_feature_db();

			//update standard deviation from the updated database
			update_std_dev(features_db);

			//check
			cv::imshow("Denoised image", bi_image_no_noise);
			cv::waitKey();
			return 0;
		}
		if (key == 'r') {
			//loading the features database
			std::vector<std::vector<std::string>>features_db = load_feature_db();

			//load std_dev;
			std::vector<float> std_dev = load_std_dev();
			
			std::cout << "\nSpecify if you would like to implement Euclidian distance - 'e' "
				<<"feature matching or Knn classifier - 'k' implementation" << std::endl;
			char classifier;
			std::cin >> classifier;
			std::string object_label;
			if (classifier == 'e') {
				//find object using least square method
				object_label = find_object(features_db, features, std_dev);
			}
			else if (classifier == 'k'){
				//find object using Knn classifier
				object_label = find_object_in_knn(features_db, features, std_dev);
			}
			else {
				std::cout << "Invalid input" << std::endl;
				return -1;
			}

			//extracting top left point of the object from the bounding box
			cv::Point P(stats.at<int>((int)features[0], cv::CC_STAT_LEFT), stats.at<int>((int)features[0], cv::CC_STAT_TOP));
			cv::putText(image, object_label, P, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0));

			out_image = image;
			cv::imshow("Recognized object", out_image);
			cv::imshow("Denoised image", bi_image_no_noise);
			cv::waitKey();
		}
	}
	//Video mode
	else if (mode == 'v') {
		cv::VideoCapture* vidsrc;

		//defining frame variable
		cv::Mat frame;
		char classifier;
		char key;
		std::vector<std::vector<std::string>>features_db;
		std::vector<float> std_dev;
		
		std::cout << "Specify if you would like to recognise - 'r' an object in image"
			<< " \nOr if you want to store features - 's' in the database" 
			<< "\nPress d for video output and 'q' to exit the video" << std::endl;
		std::cin >> key;

		if (key == 'r'){
			std::cout << "\nSpecify if you would like to implement Euclidian distance - 'e' "
				<< "\nfeature matching or Knn classifier - 'k' implementation" << std::endl;
			std::cin >> classifier;
			
			//Load std_dev;
			 std_dev = load_std_dev();
		}

		//loading the features database
		features_db = load_feature_db();

		std::string capture_name = "Screenshot";

		//capturing video via webcam
		vidsrc = new cv::VideoCapture(1);
		//Camera testing
		if (!vidsrc->isOpened()) {
			printf("Unable to open video device\n");
			return(-1);
		}

		// properties of video feed
		cv::Size refS((int)vidsrc->get(cv::CAP_PROP_FRAME_WIDTH),
			(int)vidsrc->get(cv::CAP_PROP_FRAME_HEIGHT));
		printf("Expected size: %d %d\n", refS.width, refS.height);

		cv::namedWindow("Video", 1); // identifies a window

		for (;;) {
			cv::Mat out_frame;
			char quit = cv::waitKey(10);
			*vidsrc >> frame; // get a new frame from the camera, treat as a stream
			if (frame.empty()) {
				printf("frame is empty\n");
				break;
			}

			//blurring image for better thresholding
			blur5x5(frame, blur_image);

			//converting to grayscale image
			cv::cvtColor(blur_image, gray_im, cv::COLOR_BGR2GRAY);

			//thresholding image
			bi_image_noisy = threshold(gray_im);

			//cleaning the image using opencv_func
			denoised(bi_image_noisy, bi_image_no_noise);

			//segmentation using opencv_func
			cv::Mat stats, label, centroid;
			cv::Mat inv;
			cv::bitwise_not(bi_image_noisy, inv);
			int ncomps = cv::connectedComponentsWithStats(inv, label, stats, centroid, 8, CV_16U);

			//Find the features of in the image
			std::vector<float> features = get_features(bi_image_no_noise, ncomps, label, stats);

			//default
			if (key != 'r' and key == 'd') {
				out_frame = frame;
			}
			//storing features for one frame
			if (key == 's') {
				//Storing features in a csv for later use
				update_database(features);

				//loading the features database
				std::vector<std::vector<std::string>>features_db = load_feature_db();

				//Update standard deviation from the updated database
				update_std_dev(features_db);

				//storing features just once
				key = 'd';
			}
			if (key == 'r') {
				std::string object_label;
				if (classifier == 'e') {
					//Find object using least square method
					object_label = find_object(features_db, features, std_dev);
				}
				else if (classifier == 'k') {
					//Find object using Knn classifier
					object_label = find_object_in_knn(features_db, features, std_dev);
				}
				else {
					std::cout << "Invalid input" << std::endl;
					return -1;
				}
				//extracting top left point of the object from the bounding box
				cv::Point P(stats.at<int>((int)features[0], cv::CC_STAT_LEFT), stats.at<int>((int)features[0], cv::CC_STAT_TOP));
				cv::putText(frame, object_label, P, cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 0, 0));

				out_frame = frame;
			}

			if (quit == 'q') {
				std::cout << "Exiting the interface";
				break;
			}
			//std::cout << "EVERYTHING WORKING" << std::endl;
			cv::imshow("Denoised", bi_image_no_noise);
			cv::imshow("Video", out_frame);
		}
		delete vidsrc;
	}
	else {
		 std::cout << "Invalid key" << std::endl;
	}
	return 0;
}
