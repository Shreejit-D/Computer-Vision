/*
Creator - Shreejit Deshmukh
Pattern Recognization and Computer Vision Spring 2023
Live Video Filtering
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "filter.h"

int main(int argc, char* argv[]) {
    cv::VideoCapture* vidsrc;
    int n = 1;                          //this is image name identifier for saving multiple extensions for an image

    //capturing video via webcam
	vidsrc = new cv::VideoCapture(0);

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
    
    //defining frame variable
    cv::Mat frame;
    int flag = 0; //default flag for rgb format
    char ans = 'y';
    int c = 5;
    int level; 
    int magthreshold;
    std::string capture_name = "Screenshot";
    
    std::cout << "This is a code which converts live-stream web cam video into live filtering implementation \nUse the following keys for implementation of filters ->\n\n'r' for reverting back to RGB format\n'g' for grayscale image conversion \n'h' for alternate grayscale image conversion \n'b' for Gaussian blurring\n'x' for x-direction Sobel filtering\n'y' for y-direction Sobel filtering\n'm' for gradiant magnitude image\n'i' for blurred quantisized image \n'c' for cartoonized effect \n'n' for negative filtering of live image\n'o' for awesome extension of turning skin color to white (semi naked human detection :P)" << std::endl;
    for (;;) {
        cv::Mat out_frame;

        *vidsrc >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }       
        
        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        
        //changing flags with keystrokes
        switch (key) {
        case 'g':   //grayscale image default inbuilt CV function
            flag = 1;
            std::cout << "Switching to grayscale effect" << std::endl;
            capture_name = "grayscale_normal";
            break;  //alternate grayscale image using user-defined values
        case 'h':
            flag = 2;
            std::cout << "Switching to alternate grayscale effect" << std::endl;
            capture_name = "grayscale_alternate";
            break;
        case 'r':   //reset to RGB format
            flag = 0;
            std::cout << "\nReseting the to RGB effect" << std::endl;
            capture_name = "RGB_normal";
            out_frame = frame;
            break;
        //case 'b':   //gaussian blurring -> currently advanced gaussian enabled can uncomment for single gaussian
        //    flag = 3;
        //    std::cout << "Switching to blurring effect" << std::endl;
        //    break;
        case 'x':   //X direction sobel filtering 
            std::cout << "Switching to X sobel filtering effect" << std::endl;
            flag = 4;
            capture_name = "X_dir_sobel_image";
            break;
        case 'y':   //Y direction sobel filtering
            std::cout << "Switching to Y sobel filtering effect" << std::endl;
            flag = 5;
            capture_name = "Y_dir_sobel_image";
            break;
        case 'm':   //Gradiant magnitude image
            std::cout << "Switching to gradiant magnitude effect" << std::endl;
            flag = 6;
            capture_name = "Gradiant_magnitude_image";
            break;
        case 'n':   //negative image filtering - 10q
            std::cout << "Switching to negative image effect" << std::endl;
            flag = 9;
            capture_name = "negative_image";
            break;
        case 'o':   //face whitening
            std::cout << "Switching to skin color (hooman) detection mode" << std::endl;
            flag = 20;
            capture_name = "Extension_image";
        default:
            out_frame = frame;
            break;
        }
        if (key == 'i') {
            std::cout << "Enter value for blurring effect -> ";
            std::cin >> level;
            if (level > 255) {
                std::cout << "invalid input";     //sanity check
                return(-1);
            }
            flag = 7;
            capture_name = "Blurred_Quantisized_image";
        }
        if (key == 'c') {
            std::cout << "Enter value for blurring effect -> ";
            std::cin >> level;
            if (level > 255) {
                std::cout << "invalid input";     //sanity check
                return(-1);
            }
            std::cout << "Enter threshold value for blackening the values -> ";
            std::cin >> magthreshold;
            if (magthreshold > 255) {
                std::cout << "invalid input";     //sanity check
                return(-1);
            }
            capture_name = "Cartoonized_image";
            flag = 8;
        }


        //Extension 1 - x times blurring
        if (key == 'b') {
            int x = 1;
            std::cout << "Enter x to see x times blurring effect\nmax = 5; by default = 1 - ";
            std::cin >> x;
            flag = 12;
            c = x;
            capture_name = "Gaussian_Blurred_image";
        }

        if (flag == 12){
            if (c > 5 or c < 0) {
                std::cout << "\nInvalid input"; //sanity check
                return(-1);
            }
            cv::Mat xblur;

            blur5x5(frame, xblur);

            for (int i = 0; i < c - 1; i++) {       //implementing gaussian on the same frame c - 1 times
                blur5x5(xblur,xblur);
            }
            out_frame = xblur;
        }


        //Using flag values to change the filter
        if (flag == 1) {
            cv::cvtColor(frame, out_frame, cv::COLOR_BGR2GRAY);
        }
        if (flag == 2) {
            greyscale(frame, out_frame);

        }
        /*if (flag == 3) {
            blur5x5(frame, out_frame);
        }*/
        if (flag == 4) {
            cv::Mat sx;
            sobelX3x3(frame, sx);
            cv::convertScaleAbs(sx, out_frame);     //for imread we need values between [0 255] but sobel filter gives out [-255 255]
        }
        if (flag == 5) {
            cv::Mat sy;
            sobelY3x3(frame, sy);
            cv::convertScaleAbs(sy, out_frame);     //for imread we need values between [0 255] but sobel filter gives out [-255 255]
        }
        if (flag == 6) {
            cv::Mat S_x_s, S_y_s, S_x, S_y, out_1, out_2;
            sobelX3x3(frame, S_x);
            sobelY3x3(frame, S_y);
            //cv::convertScaleAbs(S_x_s, S_x);
            //cv::convertScaleAbs(S_y_s, S_y);

            magnitude(S_x, S_y, out_1);
            cv::convertScaleAbs(out_1, out_frame);
        }
        if (flag == 7) {
            blurQuantisize(frame, out_frame, level);
        }
        if (flag == 8) {
            cartoon(frame, out_frame, level, magthreshold);
        }
        if (flag == 9) {
            negative(frame, out_frame);
        }
        
        //Extension 2 - skin color to white
        if (flag == 20) {
            whiteboi(frame, out_frame);
        }

        //For saving
        if (key == 's') {
            cv::String save_name;
            save_name = capture_name + std::to_string(n) + ".jpg";
            std::cout << "\nSaving image " + save_name << std::endl;
            cv::imwrite(save_name, out_frame);
            n++;
        }
        //For quiting
        if (key == 'q') {
            break;
        }

        cv::imshow("Video", out_frame);
    }

    delete vidsrc;
    return(0);
}

