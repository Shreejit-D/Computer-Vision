# Real-Time Video Filtering (C++)

## Introduction

This project, *Real-Time Filtering*, focuses on implementing various filters (both built-in and user-defined) on frames captured from a video stream using OpenCV in C++. The project is structured into multiple tasks, each designed to deepen the understanding of pixel based filtering from scratch.

### Screenshots from real time filtering -
![image](https://github.com/user-attachments/assets/73b2892e-9688-49d9-8acf-5167dc6af488)
![image](https://github.com/user-attachments/assets/61938d3e-a29c-4a1d-84a6-8626affce89c)
![image](https://github.com/user-attachments/assets/95c5588d-f42a-43db-b667-a940d0374312)

## Features

### 1. Grayscale Conversion
- **Function:** `int greyscale(cv::Mat& src, cv::Mat& dst)`
- **Description:** Converts an RGB image to grayscale by averaging the RGB intensity values.

### 2. Gaussian Blur
- **Function:** `int blur5x5(cv::Mat& src, cv::Mat& dst)`
- **Description:** Applies Gaussian blur using separable 5x5 matrices for smoothing the image.

### 3. Sobel Filtering
- **Functions:**
  - `int sobelX3x3(cv::Mat& src, cv::Mat& dst)`
  - `int sobelY3x3(cv::Mat& src, cv::Mat& dst)`
- **Description:** Detects edges in the X and Y directions using the Sobel operator.

### 4. Magnitude Calculation
- **Function:** `int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst)`
- **Description:** Calculates the magnitude of the gradient from the Sobel X and Y filters.

### 5. Blurring and Quantization
- **Function:** `int blurQuantisize(cv::Mat& src, cv::Mat& dst, int level)`
- **Description:** Blurs the image and quantizes the color values based on the specified level.

### 6. Cartoon Effect
- **Function:** `int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold)`
- **Description:** Applies a cartoon-like effect by combining quantization with edge detection.

### 7. Negative Filter
- **Function:** `int negative(cv::Mat& src, cv::Mat& dst)`
- **Description:** Inverts the colors of the video stream, creating a negative effect.

### 8. Skin Color Detection (HSV)
- **Function:** `int whiteboi(cv::Mat& src, cv::Mat& dst)`
- **Description:** Detects skin-like colors in the image using the HSV color space and highlights them.

## How to Run

### Prerequisites
- **C++ Compiler** (e.g., GCC, MSVC)
- **OpenCV Library** (version 4.x or higher)
