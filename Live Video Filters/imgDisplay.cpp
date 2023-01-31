#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
	
	if (argc != 2)
	{
		std::cout << "Wrong number of parameters" << std::endl;		//sanity check
		return (-1);
	}

	cv::Mat image = cv::imread(argv[1], 1);
	cv::namedWindow("image", 1);
	imshow("image", image);
	while(1)
	{
		char key = cv::waitKey(0);
		if (key == 'q') {
			cv::destroyWindow("image");
			break;
		}
	}
	return (0);
}

