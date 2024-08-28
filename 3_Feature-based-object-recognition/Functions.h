#pragma once
/*
@Creator - Shreejit Gajanan Deshmukh
@PRCV assignment 3 - Real-time object recognition
*/
#define _USE_MATH_DEFINES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <fstream>



/********************************************************************
All the functions for our main code are written here, with parameter*
instructions given above the function.								*
********************************************************************/

//Using a 5X5 Gaussian filtering on our input image for efficient binary\\
conversion of our input image.
int blur5x5(cv::Mat& src, cv::Mat& dst) {
	cv::Mat dst_c;
	src.copyTo(dst_c);	//so that we have same 1st and last rows and columns

	//Implementing seperable gaussian filtering 1st row wise [1 2 4 2 1] then column wise [1 2 4 2 1]T
	for (int i = 2; i < src.rows - 2; i++) {
		uchar* r_sptr = src.ptr<uchar>(i);
		uchar* r_dptr = dst_c.ptr<uchar>(i);

		for (int j = 2; j < src.cols - 2; j++) {
			r_dptr[j] = (r_sptr[j - 2] * 1 + r_sptr[j - 1] * 2 + r_sptr[j] * 4 + r_sptr[j + 1] * 2 + r_sptr[j + 2] * 1) / 10;
		}
	}

	cv::Mat dst_c2;
	dst_c.copyTo(dst_c2);

	for (int i = 2; i < src.rows - 2; i++) {
		uchar* r_sptr = dst_c.ptr<uchar>(i);
		uchar* r_dptr = dst_c2.ptr<uchar>(i);
		//defining top 2 rows and bottom 2 rows for row of interest
		uchar* r_sm2ptr = dst_c.ptr<uchar>(i - 2);	//-2nd row
		uchar* r_sm1ptr = dst_c.ptr<uchar>(i - 1);	//-1st row
		uchar* r_sp1ptr = dst_c.ptr<uchar>(i + 1);	//+1st row
		uchar* r_sp2ptr = dst_c.ptr<uchar>(i + 2);		//+2nd row

		for (int j = 2; j < src.cols - 2; j++) {
			r_dptr[j] = (r_sm2ptr[j] * 1 + r_sm1ptr[j] * 2 + r_sptr[j] * 4 + r_sp1ptr[j] * 2 + r_sp2ptr[j] * 1) / 10; //divided by 10 as it can be the max value
		}
	}
	cv::convertScaleAbs(dst_c2, dst);
	return(0);
}


//Custom user defined thresholding function which converts any pixel above\\
128 to white (background) and our object of interest to black. Input is   \\
grayscale image and output is the binary image with values 0 and 255	  
cv::Mat threshold(cv::Mat& image) {
	int threshold_val = 128;					//divides the image into dark and light regions
	cv::Mat bin_image(image.size(),CV_8U);
	for (int i = 0; i < image.rows; i++) {
		uchar* rptr_s = image.ptr<uchar>(i);
		uchar* rptr_d = bin_image.ptr<uchar>(i);
		for (int j = 0; j < image.cols; j++) {
			if (rptr_s[j] < threshold_val) {
				rptr_d[j] = 0;
			}
			else {
				rptr_d[j] = 255;
			}
		}
	}
	return bin_image;
}

//Color segmenting done to identify different regions and color it in our binary image\\
the input is a labeled binary image with different components and the output is a RBG \\
image
void color_segments(cv::Mat label, cv::Mat stats, int ncomps) {
	cv::Mat color_seg;
	color_seg = cv::Mat::zeros(label.size(), CV_16UC3);
	//std::cout << "\nNumber of components - " << ncomps;
	for (int n = 1; n < ncomps; n++) {						//excluding background
		if (stats.at<int>(n, cv::CC_STAT_AREA) > 1000) {	
			std::cout << "\nFor component n" << n;
			ushort r, g, b, ran;
			ran = rand() % 10;
			r = 255 / ran;
			g = 255 / ran;
			b = 255 / ran;
			for (int j = 0; j < color_seg.rows; j++) {
				for (int k = 0; k < color_seg.cols; k++) {
					if (label.at<ushort>(j, k) == n) {
						color_seg.at<ushort>(j, k, 0) = b;
						color_seg.at<ushort>(j, k, 0) = g;
						color_seg.at<ushort>(j, k, 0) = r;
					}
				}
			}
		}
	}
	cv::convertScaleAbs(color_seg, color_seg);
	cv::namedWindow("Components", 1);
	cv::imshow("Components", color_seg);
	cv::waitKey();
}

//This function takes our converted binary image as input and converts it to\\
denoised image which removes all present holes from the object of interest.
void denoised(cv::Mat& image, cv::Mat& output) {
	cv::Mat inv;
	cv::bitwise_not(image, inv);

	// A image with size greater than the present object is created, it is needed for floodFill()
	cv::Mat mask = cv::Mat::zeros(image.rows + 2, image.cols + 2, CV_8U);

	cv::floodFill(inv, mask, cv::Point(0, 0), 255, 0, cv::Scalar(), cv::Scalar(), 4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);

	//remove the extra rows/cols added earlier in the initialization of the mask.
	mask(cv::Range(1, mask.rows - 1), cv::Range(1, mask.cols - 1)).copyTo(output);
}

//This function displays the bounding box around our object and highlights\\
the minor axis of our object of interest. Input is our RGB image along with\\
labels, statistics, and number of components in our binary image. Output is\\
our image updated with the bounding box and minor axis.
void display_box(cv::Mat image, cv::Mat label, cv::Mat stats, int ncomps) {
	for (int n = 1; n < ncomps; n++) {
		if (stats.at<int>(n, cv::CC_STAT_AREA) > 1000) {
			double nu02, nu20, nu11, alpha;
			cv::Mat mask = (label == n);
			cv::Mat query_image;
			std::vector<cv::Point> Obj_pts;

			std::cout << "\nArea in the mask is - " << stats.at<int>(n, cv::CC_STAT_AREA);
			
			for (int i = 0; i < mask.rows; i++) {
				for (int j = 0; j < mask.cols; j++) {
					if (mask.at<ushort>(i, j) == n) {
						mask.at<ushort>(i, j) = 255;
					}
				}
			}

			cv::convertScaleAbs(mask, query_image);
			/*cv::imshow("mask", query_image);
			cv::waitKey();*/

			cv::Moments moment = cv::moments(mask);
			
			//normalized moments for object i
			nu11 = moment.nu11;
			nu02 = moment.nu02;
			nu20 = moment.nu20;

			//angle for least central moment
			double val = (2 * nu11) / (nu20 - nu02 + 0.000001);
			alpha = std::atan(val) / 2;
			float beta = alpha + atan(1) * 2;
			std::cout << "\n difference of pi/2 in c++ - " << alpha - beta;

			std::cout << "\nAngle for the minor axis for comp "<< n << " " << (alpha * 180) / 3.14;

			cv::Point cen(moment.m10 / moment.m00, moment.m01 / moment.m00);	//centroid of object
			
			std::vector<int> len;
			std::vector<cv::Point> axis_pts;
			std::cout << "\nCenter is " << cen.x << "," << cen.y;
			//traversing to find the length and width of bounding box
			for(int i = 0; i < 2; i++){
				cv::Point N(cen);
				while (1) {
					float s = sin(alpha + i*(atan(1)*2));
					float c = cos(alpha + i*(atan(1)*2));
					N.x = N.x + 5 * (c);
					N.y = N.y + 5 * (s);
					/*std::cout << "\nlabel at center " << (int)query_image.at<uchar>(cen);
					std::cout << "\nnext point " << N.x << "," << N.y;
					std::cout << "\nlabel at next point " << (int)query_image.at<uchar>(N);*/
					if ((int)query_image.at<uchar>(N) == 0) {
						break;
					}
					//cv::circle(image, N, 2, (0, 0, 255));
				}
				int length = std::sqrt((N.x - cen.x) * (N.x - cen.x) + (N.y - cen.y) * (N.y - cen.y));  //some additional length to protrude it outwards
				std::cout << "\nlength - " << length << " ";
				len.push_back(length);
				axis_pts.push_back(N);
			}
			//Draw minimum moment axis
			cv::Point p1 = axis_pts[0];
			cv::Point p2 = 2*cen - p1;			//midpt theorem
			cv::line(image, p1, p2, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);

			//highlighting centroid
			cv::circle(image, cen, 2, cv::Scalar(0,0,255));
			
			//for finding minimum area oriented rectangle
			for (int i = 0; i < query_image.rows; i++) {
				for (int j = 0; j < query_image.cols; j++) {
					if ((int)query_image.at<uchar>(i, j) == 255) {
						Obj_pts.push_back(cv::Point(j, i));
					}
				}
			}

			cv::RotatedRect box = cv::minAreaRect(Obj_pts);
			cv::Point2f vtx[4];
			box.points(vtx);

			// Draw the bounding box
			for (int i = 0; i < 4; i++) {
				line(image, vtx[i], vtx[(i + 1) % 4], cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
			}

			cv::namedWindow("image", 1);
			cv::imshow("image", image);
			cv::waitKey();
		}
	}
}

//This function is to get our comparison features from the extracted object\\
from our binary image. The input is the binary image and the output is a   \\
a vector of features which are hu_moments. A detailed description is attached\\
in the report
std::vector<float> get_features(cv::Mat bin_image, int ncomponents, cv::Mat label, cv::Mat stats) {
	int max_area = 0; int max_index = 100;

	//finding most prominant component
	for (int i = 1; i < ncomponents; i++) {
		if (stats.at<int>(i, cv::CC_STAT_AREA) > max_area) {
			max_area = stats.at<int>(i, cv::CC_STAT_AREA);
			max_index = i;
		}
	}

	//features from left to right - normalized spatial moment, normalized inertial moment in y, normalized inertial moment in x\\
	, area, bounding box height, bounding box width)
	double per_filled, area, bb_h, bb_w;
	//extracting 1 component at one time
	cv::Mat mask = (label == max_index);

	//calculating hu moments
	cv::Moments moment = cv::moments(mask);
	double hu_moments[7];
	HuMoments(moment, hu_moments);

	area = stats.at<int>(max_index, cv::CC_STAT_AREA);
	bb_h = stats.at<int>(max_index, cv::CC_STAT_HEIGHT);
	bb_w = stats.at<int>(max_index, cv::CC_STAT_WIDTH);
	per_filled = area / (bb_h * bb_w);


	//defining features for each component with i as component identifier, total 8 features for comparison
	std::vector<float>features;
	std::vector<float>h_m;
	
	for (int i = 0; i < 6; i++) {
		float k = std::log10(hu_moments[i]);
		//detecting division by zero if present and changing it to 0 since\\
		its contribution will be insignificant
		if (std::isnan(k)){
			k = 0;
		}
		h_m.push_back(k);
	}

	features.push_back(max_index); features.push_back(area); features.push_back(bb_h); features.push_back(bb_w); 
	features.push_back(h_m[0]);	features.push_back(h_m[1]); features.push_back(h_m[2]); features.push_back(h_m[3]); 
	features.push_back(h_m[4]); features.push_back(per_filled);
		
	//Checking features
	/*std::cout << "\nFeatures for the object are -";
	for (float i : features) {
		std::cout << " " << i;
	}*/

	return(features);
}

//This function updates our .csv with the features extracted fo the object \\
of interest. The input is the feature vector and it updates the .csv file 
int update_database(std::vector<float> feature_vec) {
	char name_tag[50];

	std::cout << "\nInput label (identifier) of the object - " << std::endl;
	std::cin >> name_tag;

	std::ofstream outfile("database.csv", std::ios_base::app | std::ios_base::out);

	// Write the list elements to the CSV file
	outfile << name_tag << ",";
	for (float i : feature_vec) {
		outfile << i << ",";
	}
	outfile << std::endl;
	outfile.close();

	std::cout << "\nCSV file updated with new component and its features!" << std::endl;

	return 0;
}

//This function loads the feature database from the excel to a vector<vector>\\
for object recognition. The output is the loaded features from the database.
std::vector<std::vector<std::string>> load_feature_db() {
	std::vector<std::vector<std::string>> feature_db;

	// Open the input file stream
	std::ifstream file("database.csv");

	// Read the file line by line
	std::string line;
	while (getline(file, line)) {
		std::vector<std::string> record;
		std::stringstream ss(line);
		std::string field;

		// Split the line by commas and store each field in the record vector
		while (std::getline(ss, field, ',')) {
			record.push_back(field);
		}

		feature_db.push_back(record);
	}

	// Close the file stream
	file.close();

	return feature_db;
}

//This function updates our std deviation .csv file with any new updated features\\
whenever we are storing a new object for recognition
int update_std_dev(const std::vector<std::vector<std::string>>& feature_db) {
	//loading standard deviation size excluding label, area, height, and width
	int size_sd = feature_db[0].size() - 5;
	std::vector<float> std_dev(size_sd, 0.0);
	int num_records = feature_db.size();

	//calculate the mean for each parameter
	std::vector<double> means(size_sd, 0.0);
	for (int j = 5; j < size_sd + 5; j++) {
		double sum = 0.0;
		for (int i = 0; i < num_records; i++) {
			double val = std::stod(feature_db[i][j]);
			sum += val;
		}
		means[j - 5] = sum / num_records;
	}

	//calculate the sum of squared differences from the mean
	std::vector<double> sum_diff_sq(size_sd, 0.0);
	for (int j = 5; j < size_sd + 5; j++) {
		for (int i = 0; i < num_records; i++) {
			double val = std::stod(feature_db[i][j]);
			double diff = val - means[j - 5];
			sum_diff_sq[j - 5] += diff * diff;
		}
	}

	//calculate the variance and standard deviation
	for (int j = 5; j < size_sd + 5; j++) {
		double variance = sum_diff_sq[j - 5] / num_records;
		std_dev[j - 5] = sqrt(variance);
	}

	//detecting division by zero if present and changing it to 0 since\\
	its contribution will be insignificant
	//double check
	for (int i = 0; i < std_dev.size(); i++) {
		if (std::isnan(std_dev[i])) {
			std_dev[i] = 0;
		}
	}

	//storing it to a csv file so that we don't have to calculate it again for each computation
	std::ofstream file("std_dev.csv", std::ios_base::trunc);
	if (!file.is_open()) {
		std::cout << "Error in opening the file";
		return -1;
	}

	//Value check
	std::cout << "\nUpdated Standard deviations values - ";
	for (float i : std_dev) {
		std::cout << i << " ";
	}

	//Writing in CSV
	for (float i : std_dev) {
		file << i << ",";
	}

	file << std::endl;
	file.close();
	std::cout << "\nStandard deviation file has been updated";
	return 0;
}

//This function load the standard deviation in a vector which is used later for\\
feature normalizing while calculating euclidian distances.
std::vector<float> load_std_dev() {
	std::vector<float> err;
	std::ifstream input_file("std_dev.csv");

	if (!input_file.is_open()) {
		std::cerr << "Error opening input file!\n";
		return err;
	}

	std::string line;
	std::getline(input_file, line);

	std::stringstream ss(line);
	std::vector<std::string> std_dev_str;

	while (ss.good()) {
		std::string substr;
		std::getline(ss, substr, ',');
		std_dev_str.push_back(substr);
	}

	std::vector<float> std_dev(6, 0);

	//converting to double for our operations - tried for loop but was throwing errors
	std_dev[0] = stod(std_dev_str[0]);
	std_dev[1] = stod(std_dev_str[1]);
	std_dev[2] = stod(std_dev_str[2]);
	std_dev[3] = stod(std_dev_str[3]);
	std_dev[4] = stod(std_dev_str[4]);
	std_dev[5] = stod(std_dev_str[5]);

	//check for values
	/*std::cout << "\nloaded standard deviations for the database are - ";
	for (float i : std_dev) {
		std::cout << " " << i;
	}*/

	return std_dev;
}

//This function returns the normalized least euclidian distances of two vector features
float least_euc_dis(std::vector<float> vec1, std::vector<float> vec2, std::vector<float> std_dev) {
	float dist = 0;
	//excluding percentage filled as it was giving bad results for rotations
	for (int i = 0; i < vec1.size() - 1; i++) {
		//std::cout << "\n" << vec1[i] << " and " << vec2[i];
		dist = (vec1[i] - vec2[i])/std_dev[i];
	}
	return(std::abs(dist));
}

//This function finds least euclidian distance for each feature of our object to the features\\
from our database objects and returns the label for the most similar object.
std::string find_object(std::vector<std::vector<std::string>> &feature_db, std::vector<float> &features, std::vector<float> &std_dev) {
	int size_of_db = feature_db.size();
	int num_features = std_dev.size();

	//taking out features from feature db and labels seperately
	std::vector<std::vector<float>> features_from_db(size_of_db, std::vector<float>(num_features));
	std::vector<std::string> labels;
	
	//database features
	for (int i = 0; i < size_of_db; i++) {
		for (int j = 0; j < num_features; j++) {
			features_from_db[i][j] = stod(feature_db[i][j + 5]); // exclude the label, area, height, and base for comparison 
		}
	}
	//database labels
	for (std::vector<std::string> i : feature_db) {
		labels.push_back(i[0]);
	}
	
	//calculating distance for each label and finding the minimum
	float dist = 0;
	float min = 100000;
	int min_ind = 1000;
	std::vector<float>features_for_comp(features.begin() + 4, features.end());
	for (int i = 0; i < size_of_db; i++) {
		dist = least_euc_dis(features_for_comp, features_from_db[i], std_dev);
		if (dist < min) {
			min = dist;
			min_ind = i;
		}
		//std::cout << "\nDist for label " << labels[i] << " - " << dist << std::endl;
	}
	std::cout << "Co-relation label found is " << labels[min_ind] << std::endl;
	return labels[min_ind];
}

//This function returns least euclidian distance for each feature of our object to the features\\
from our database objects and returns the label for the most similar object using Knn Classifier\\
The database is expected to be consecutive one below each other.
std::string find_object_in_knn(std::vector<std::vector<std::string>>& feature_db, std::vector<float>& features, std::vector<float>& std_dev) {
	int size_of_db = feature_db.size();
	int num_features = std_dev.size();

	//taking out features from feature db and labels seperately
	std::vector<std::vector<float>> features_from_db(size_of_db, std::vector<float>(num_features));
	std::vector<std::string> labels;
	//database features
	for (int i = 0; i < size_of_db; i++) {
		for (int j = 0; j < num_features; j++) {
			features_from_db[i][j] = stod(feature_db[i][j + 5]); // exclude the label, area, height, and base for comparison 
		}
	}
	//database labels
	for (std::vector<std::string> i : feature_db) {
		labels.push_back(i[0]);
	}
	//calculating distance for each label and finding the minimum
	float dist = 0;
	float prev_dist = 0;
	float min = 100000;
	int min_ind = 1000;
	int count = 1;
	std::vector<float>features_for_comp(features.begin() + 4, features.end());
	
	//Knn classifier
	for (int i = 0; i < size_of_db; i++) {
		dist = least_euc_dis(features_for_comp, features_from_db[i], std_dev);
		if (i > 0) {
			//Assuming groups of same labels are stored consecutively
			if (labels[i] == labels[i - 1]) {
				std::cout << "\nfound same label " << labels[i];
				prev_dist = prev_dist + dist;
				count++;
			}
			if (labels[i] != labels[i - 1] or i == size_of_db - 1) {
				//normalizing the end distance for comparison
				dist = prev_dist / count;
				std::cout << "\nnormalized distance for the label " << labels[i] << " is " << dist;
				prev_dist = 0;
				count = 1;
			}
		}
		if (dist < min) {
			min = dist;
			min_ind = i;
		}
	}
	std::cout << "\nCo-relation label found is " << labels[min_ind] << std::endl;
	return labels[min_ind];
	return 0;
}