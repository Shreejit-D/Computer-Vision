# Content-Based Image Retrieval (CBIR)

## Introduction

The purpose of this project is to correlate and rank images based on the features of a target image compared to other images in the dataset. These features may include pixel color, texture, or a combination of both. The project is divided into five primary tasks, each implementing different methods for image comparison and retrieval.

## Tasks Overview

### Task 1: Baseline Matching
- **Objective:** Perform a direct pixel-to-pixel RGB color intensity comparison between the target and query images.
- **Method:** A 9x9 matrix is extracted from the center of the target image, and each pixel's RGB intensity is compared to the query images using the least squares metric.
- **Results:**

![image](https://github.com/user-attachments/assets/e106b137-4745-4265-9179-69f7c58e0e47)


### Task 2: Histogram Matching
- **Objective:** Compare images based on the rg chromaticity normalized histogram.
- **Method:** For each pixel, the r and g chromaticity values are calculated and placed into 18x18 bins. Histogram intersection is used as the comparison metric.
- **Results:**

![image](https://github.com/user-attachments/assets/50d2bcd7-1714-4454-8637-6260017d8ccd)


### Task 3: Multi-Histogram Matching
- **Objective:** Improve matching by dividing the target image into four sections and matching histograms for each section separately.
- **Method:** The target and query images are divided into four quadrants, and histograms are calculated and compared for each corresponding section.
- **Results:**

![image](https://github.com/user-attachments/assets/17ad1de9-e035-4c5f-aa3f-b59543ad33e1)


### Task 4: Texture and Color Matching
- **Objective:** Combine color histogram matching with Sobel magnitude histogram matching, equally weighted.
- **Method:** Histogram intersection is used to compare both color distribution and boundary texture between the target and query images.
- **Results:**

![image](https://github.com/user-attachments/assets/f3d55e9b-3bbf-41b0-8cdc-653523c003b2)


### Task 5: Custom Design - Sky Detection
- **Objective:** Rank images based on the presence of a blue sky using a custom reward/punish metric.
- **Method:** The algorithm extracts the upper half of each image, analyzes HSV values to detect blue sky, and applies a reward/punish system to rank images.
- **Results:** The method performs well with 100% accuracy for easy and medium sets, and 75% for the difficult set, with challenges arising from non-sky blue backgrounds.

![image](https://github.com/user-attachments/assets/e646d373-3c3b-46eb-966f-81f780504138)

## Extensions

### Extension A: Collage Creation
- **Objective:** Visualize the ranking results quickly by creating a collage of the top-ranked images.
- **Method:** The collage is automatically generated and saved as an output file.

### Extension B: Banana Finder
- **Objective:** Identify the presence of a banana in the center of an image using a custom algorithm.
- **Method:** The algorithm is divided into six steps: 
  1. Center portion extraction.
  2. Gaussian blur application.
  3. Sobel magnitude application.
  4. Histogram matching with reference banana images.
  5. HSV color analysis.
  6. Weight optimization.
- **Results:** The algorithm achieved 80% accuracy for easy and medium sets and 60% for the difficult set. Challenges include noise in Sobel magnitude images and the need for more sophisticated shape recognition algorithms.


![image](https://github.com/user-attachments/assets/67eb45dc-6203-440e-a929-3123ba865c8e)


## How to Run

### Prerequisites
- **C++ Compiler** (e.g., GCC, MSVC)
- **OpenCV Library** (version 4.x or higher)
