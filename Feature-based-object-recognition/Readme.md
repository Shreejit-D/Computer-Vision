# Real-time 2D Object Recognition

## Introduction

The goal of this project is to perform real-time object recognition using shape features such as moments, which are invariant to scale, translation, and rotation. The project involves several tasks that guide the implementation of the object recognition system using custom algorithms and OpenCV functions.

## Tasks Overview

### Task 1: Thresholding the Input Video (Without OpenCV Function)
- **Objective:** Convert the input video to a binary image by thresholding, without using OpenCV's built-in functions.
- **Method:** A 2D histogram of the image is plotted and divided based on the mean value between two intensity maxima. The image is preprocessed with a Gaussian filter for better thresholding.
- **Results:** A binary image is produced that separates the object of interest from the background.

### Task 2: Cleanup of Binary Image
- **Objective:** Clean up the binary image to remove noise and fill holes in the object.
- **Method:** OpenCV's `floodfill` function is used, which fills connected regions based on a seed point and boundary conditions. This method efficiently cleans the image without performing dilation or erosion.
- **Results:** A cleaner binary image with well-defined object boundaries.

### Task 3: Image Segmentation
- **Objective:** Segment the binary image into connected components.
- **Method:** OpenCV's `cv::connectedComponentsWithStats` function is used to perform a 4-connected component analysis. This function outputs labels, stats (area, bounding box), and centroids for each component.
- **Results:** Different components in the image are identified and highlighted with different colors.

### Task 4: Computing Features for Each Major Region
- **Objective:** Extract features from the major regions in the image for object recognition.
- **Method:** The features extracted include the height, base, area, and percentage fill of the bounding box. Additionally, Hu moments (seven invariant moments) are computed for each region, providing rotation, scale, and translation invariance. The axis of least moment and oriented bounding box are also calculated.
- **Results:** Each object is characterized by its Hu moments and other features, with the axis of least moment and oriented bounding box visually highlighted.

### Task 5: Collecting Training Dataset
- **Objective:** Build a training dataset of objects using the extracted features.
- **Method:** Images are used for training as they allow easy visualization of segmentation quality. The seven Hu moments are calculated and logged, with additional parameters like percentage filled area. However, non-invariant features are excluded from the final comparison.
- **Results:** A CSV file containing the features of labeled objects is created for use in recognition tasks.

### Task 6: Classify New Images
- **Objective:** Classify new images based on the collected dataset.
- **Method:** The classifier uses the least Euclidean distance between the normalized features of new images and the features in the training dataset.
- **Results:** New objects are classified correctly, demonstrating the effectiveness of the scale, rotation, and translation invariant properties of the features.

### Task 7: Implement a Different Classifier
- **Objective:** Implement a K-Nearest Neighbors (KNN) classifier for object recognition.
- **Method:** The KNN classifier calculates the least distance between a group of similar labels and the new image's features. The Euclidean distance is normalized by the number of similar labels in the database.
- **Results:** The KNN classifier successfully labels objects, with differences in parameters due to variations in image capture conditions.

### Task 8: Evaluate Your Performance
- **Objective:** Evaluate the performance of the object recognition system.
- **Method:** A confusion matrix is created to assess image matching accuracy across all objects.
- **Results:** The accuracy of the algorithm is approximately 70%, calculated as (Correctly identified objects) / (Total objects * 2)%.

## How to Run

### Prerequisites
- **C++ Compiler** (e.g., GCC, MSVC)
- **OpenCV Library** (version 4.x or higher)
