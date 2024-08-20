/*
* Creator - Shreejit Gajanan Deshmukh, Venkata Sai Advaith Kandiraju
* Assignment 4 - Augmented Reality Part A - Calibrating camera
* Pattern Recognition and Computer Vision
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include "Header_partA.h" 

using namespace std;
using namespace cv;
/*
The program finds the inner corners of the standard checkerboard with 9X6 inner corners.
The program then captures images with checkerboard in different orientations to find the
reprojection error, distortion coefficients and the intrinsic camera matrix. It then writes 
the camera matrix and the distortion coefficients to a .csv file which will be used by 
part B of our project to project virtual objects to the checkerboard
*/
int main(int argc, char* argv[]) {
    vector<Point3f> points;  //this will be filled with 3D points
    vector<vector<Point3f>> point_list; //this will be corresponding 3D points in the checkboards
    vector<vector<Point2f>> corner_list; //this will be filled by corners list of all the checkerboards
   
    //to store how many calibration images are saved. Used later
    int calib_num = 0;
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F); //for storing distortion coefficients

    //to switch between different operations
    int flag = 0;

    vector<Mat> rvecs, tvecs; //for storing rotation and translation vectors
    
    //variable for saving multiple images
    int j = 1;
    
    VideoCapture* vidsrc;
    Mat frame;
    Mat undistorted;

    //Making 3D point list
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            Point3f P(j, -i, 0);
            points.push_back(P);
            //cout << P;
        }
        //cout << endl;
    }
    cout << endl;
    cout << "Point list size - " << points.size() << endl;
    
    //capturing video via webcam
    vidsrc = new cv::VideoCapture(0);

    //camera testing
    if (!vidsrc->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // properties of video feed
    cv::Size refS((int)vidsrc->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)vidsrc->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window

    //instructions for operation
    cout << "Different modes for operating - " << endl
        << "'s' to save the corners and display the detected corners" << endl
        << "'c' to calibrate and find the intrinsic camera matrix with its distortion coefficients" << endl
        << "'u' to observe undistored image" << endl
        << "'h' to observe live harris features on the stream" << endl
        << "'d' to see the live stream" << endl
        << "'q' to quit" << endl;

    for (;;) {
        cv::Mat out_frame;
        char key = cv::waitKey(10);
        *vidsrc >> frame; // get a new frame from the camera, treat as a stream

        //camera check
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        Size frame_size = frame.size();
        Mat camera_matrix = Mat::zeros(cv::Size(3, 3), CV_64FC1);
        //setting values of camera matrix
        camera_matrix.at<double>(0, 0) = 1;
        camera_matrix.at<double>(1, 1) = 1;
        camera_matrix.at<double>(2, 2) = 1;
        camera_matrix.at<double>(0, 2) = (double)frame.cols / 2;
        camera_matrix.at<double>(1, 2) = (double)frame.rows / 2;


        if (key == 'd' or key != 's' and key != 'q') {
            imshow("Video", frame);
            flag = 0;
        }
        if (key == 's') {
            vector<Point2f> corners; //this will be filled by the detected corners
            
            Find_Corners(frame_gray, corners);
            cout << "Corners number - " << corners.size() << endl;
            
            if (!corners.empty()) {
                //appending the newly found corners for new image
                corner_list.push_back(corners);
                cout << "Number of calibrated images - " << corner_list.size() << endl;
                calib_num = corner_list.size();
                
                //appending the corresponding 3D points
                point_list.push_back(points);
                //cout << "Number of times points is pushedback" << point_list.size() << endl;

                //check corners and highlight them using circles
                //task2
                for (Point2f i : corners) {
                    //cout << i << ",";
                    circle(frame, i, 1, Scalar(0, 0, 255), 2, 8, 0);
                }
                string cal_img = "cal_img";
                string num = to_string(j);
                string ext = ".jpg";
                string image_name = cal_img + num + ext;
                namedWindow("Points_found", 1);
                imshow("Points_found", frame);
                waitKey();
                destroyWindow("Points_found");
                imwrite(image_name, frame);
                cout << endl;
                j++;
                key = 'd';
            }
        }
        if (key == 'c') {
            double reproj_error;
            //print all the camera intrinsic matrix and distortion coefficients before and after udpate
            cout << "Camera matrix before update" << endl;
            for (int i = 0; i < camera_matrix.rows; i++) {
                for (int j = 0; j < camera_matrix.cols; j++) {
                    cout << camera_matrix.at<double>(i, j) << "\t";
                }
                cout << endl;
            }
            cout << endl;
            cout << "Distortion elements before update" << endl;
            for (int i = 0; i < distCoeffs.rows; i++) {
                for (int j = 0; j < distCoeffs.cols; j++) {
                    cout << distCoeffs.at<double>(i, j) << "\t";
                }
                cout << endl;
            }
            cout << endl;

            reproj_error = calibrateCamera(point_list, corner_list, frame_size, camera_matrix, distCoeffs, rvecs, tvecs, CALIB_FIX_ASPECT_RATIO);

            //print elements after update
            cout << "Camera matrix after the update - " << endl;
            cout << camera_matrix << endl;

            cout << "Distortion elements after update" << endl;
            cout << distCoeffs << endl;
            
            //finding the reprojection error
            cout << "Reprojection error found is - " << reproj_error << endl;

            cout << "Do you want to write the camera_matrix and distortion coefficients to a csv? - 'y' or 'n'" << endl;
            char decision;
            cin >> decision;
            if (decision == 'y') {
                ofstream file("Camera_Parameters.csv", ios::trunc);
                for (int i = 0; i < camera_matrix.rows; i++) {
                    for (int j = 0; j < camera_matrix.cols; j++) {
                        file << camera_matrix.at<double>(i, j) << ",";
                    }
                }
                for (int i = 0; i < distCoeffs.rows; i++) {
                    for (int j = 0; j < distCoeffs.cols; j++) {
                        file << distCoeffs.at<double>(i, j) << ",";
                    }
                }
                file.close();
                cout << "Writing succesfull" << endl;
            }
            else {
                cout << "Did not opt to update the Camera_Parameters csv";
            }
            key = 'd';
        }
        if (key == 'u' and calib_num > 5) {
            cout << "Showing the undistorted image using calibration values" << endl;
            undistort(frame, undistorted, camera_matrix, distCoeffs);
            imshow("Undistorted", undistorted);
            waitKey();
            destroyWindow("Undistorted");
        }
        //task 7
        if (key == 'h') {
            Mat dst;
            //find harris corners in the image
            cornerHarris(frame_gray, dst, 2, 3, 0.04);
            
            //cout << dst.size() << endl;

            //normalize corner response values
            cv::Mat dst_norm;
            cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_8UC1);

            // Draw circles around corners
            for (int i = 0; i < dst_norm.rows; i++) {
                for (int j = 0; j < dst_norm.cols; j++) {
                    if ((int)dst_norm.at<uchar>(i, j) > 200) {
                        cv::circle(frame, cv::Point(j, i), 5, cv::Scalar(0), 2, 8, 0);
                    }
                }
            }
            imshow("Harris corners", frame);
            waitKey();
            destroyWindow("Harris corners");
        }

        if (key == 'q') {
            break;
        }
    }
    destroyAllWindows();
    return 0;
}