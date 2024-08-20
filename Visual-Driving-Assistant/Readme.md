# Visual Driving Assistant (VDA)

## Abstract
In this project, we present a comprehensive Visual Driving Assistant (VDA) that combines lane detection and object recognition to enhance road safety. The system employs the state-of-the-art YOLOv8 model for object detection, specifically targeting vehicles within the field of view. For lane detection, a series of image processing techniques, including Canny edge detection, Hough transforms, and masking, are utilized to identify and draw the left and right lanes. The system distinguishes between vehicles on the left, right, and ahead, issuing alerts to the driver when a vehicle is deemed too close. The results of both lane detection and object recognition are combined and visualized in real-time video output, showcasing the system's potential as a valuable driving aid. This work demonstrates the effectiveness of combining advanced deep learning techniques with traditional computer vision algorithms to enhance the driving experience and promote safety on the road.

### Algorithm in action -

https://github.com/user-attachments/assets/24e6d6e6-cdae-4bbf-8853-a1106f8fb4a5

## I. Introduction
With the increasing number of vehicles on the road, driving safety has become a critical concern. Visual Driving Assistant (VDA) systems are being developed to intelligently analyze the environment, support drivers, and promote safer driving habits. This project proposes a comprehensive VDA that combines lane detection and object recognition to enhance driving safety.

The system utilizes the YOLOv8 model for object recognition, specifically trained to identify vehicles. Simultaneously, image processing techniques such as Canny edge detection, Hough transforms, and masking are employed for lane detection. The combination of these methods allows the system to provide real-time visual feedback and alerts, helping drivers maintain safe driving conditions.

## II. Related Work

### A. Canny Edge Detection
Canny edge detection is employed to identify edges in images, crucial for detecting lane boundaries. It involves several stages: noise reduction via Gaussian filter, gradient magnitude and direction calculation, non-maximum suppression, and double thresholding to differentiate strong, weak, and non-edges.

### B. Hough Transform
Hough Transform is used to detect lines in the image after edge detection. The algorithm transforms the image from Cartesian space to parameter space, identifying lines that meet a specified threshold of votes. These lines are then processed to segregate them into left and right lanes, helping the system draw representative lane lines.

### C. YOLOv8n
YOLOv8n is a deep learning-based object detection model known for its high accuracy and speed. It identifies and tracks vehicles in real-time, providing information on the number of cars on the left, right, and ahead, and alerts for nearby vehicles that could pose a potential hazard.

## III. System Overview

### Lane Detection Pipeline
1. **Preprocessing:** The input image is converted to grayscale and smoothed using a Gaussian blur to reduce noise.
2. **Edge Detection:** Canny edge detection is applied to identify lane boundaries.
3. **Line Detection:** Hough Transform is used to detect and classify lane lines as left or right based on their slope.
4. **Lane Drawing:** Representative lines for left and right lanes are drawn on the original image.

### Object Recognition Pipeline
1. **Vehicle Detection:** YOLOv8n detects vehicles in the scene.
2. **Proximity Analysis:** The system analyzes the position and proximity of detected vehicles to provide relevant information to the driver.
3. **Alert System:** Alerts are issued when a vehicle is too close, helping the driver maintain a safe distance.

### Integration
The results of lane detection and object recognition are combined and visualized in real-time, providing comprehensive feedback to the driver.

## IV. Experiments and Results

### A. Experiments
The system was tested on three datasets from the Udacity database: `solidWhiteRight.mp4`, `solidYellowLeft.mp4`, and `challenge.mp4`. Each dataset presented unique challenges, such as different lane types and lighting conditions.

### B. Results

#### Lane Detection Errors
- **Incorrect Slope:** Occurs when the left or right lane is incorrectly recognized, potentially leading to hazardous situations.
- **Cross-Poly:** Occurs when points from opposite slopes mix, forming a cross-polygon, especially on curvilinear roads.
- **Slope Not Detected:** Occurs when the lane detection fails due to irregular point distribution.

#### Object Recognition Errors
- **Incorrect Classification:** Occurs when a non-vehicle object is incorrectly classified as a vehicle.
- **Incorrect Information:** Occurs when a far vehicle is incorrectly classified as a near one, potentially due to the vehicle's position in the camera's field of view.

### Performance Metrics
- Lane Detection Error (LDE) is calculated using weights assigned to different types of errors. The total LDE for `solidWhiteRight.mp4` is ~0.95%, for `solidYellowLeft.mp4` is ~2.32%, and for `challenge.mp4` is ~43.98%.
- Vehicle recognition errors were observed with an error rate of ~13% across all videos.

## V. Conclusion
The Visual Driving Assistant (VDA) system effectively combines traditional computer vision techniques with deep learning for real-time lane detection and object recognition. While the system performs well in most scenarios, challenges remain in complex road conditions and lighting variations. Future work could explore the integration of deep learning-based lane detection methods, such as semantic segmentation, to improve accuracy in challenging environments.

---
