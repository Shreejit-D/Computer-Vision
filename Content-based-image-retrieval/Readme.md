# Content-Based Image Retrieval (CBIR)

## Introduction

The purpose of this project is to correlate and rank images based on the features of a target image compared to other images in the dataset. These features may include pixel color, texture, or a combination of both. The project is divided into five primary tasks, each implementing different methods for image comparison and retrieval.

## Tasks Overview

### Task 1: Baseline Matching
- **Objective:** Perform a direct pixel-to-pixel RGB color intensity comparison between the target and query images.
- **Method:** A 9x9 matrix is extracted from the center of the target image, and each pixel's RGB intensity is compared to the query images using the least squares metric.
- **Results:** The top 3 results for the image `pic.1016.jpg` demonstrate that red color dominates the ranked images, reflecting the red dominance in the target image.

### Task 2: Histogram Matching
- **Objective:** Compare images based on the rg chromaticity normalized histogram.
- **Method:** For each pixel, the r and g chromaticity values are calculated and placed into 18x18 bins. Histogram intersection is used as the comparison metric.
- **Results:** For `pic.0164.jpg` as the target, the results show images with a similar proportion of blue and green.

### Task 3: Multi-Histogram Matching
- **Objective:** Improve matching by dividing the target image into four sections and matching histograms for each section separately.
- **Method:** The target and query images are divided into four quadrants, and histograms are calculated and compared for each corresponding section.
- **Results:** For `pic.0274.jpg`, the results are visually more correlated, with similar sectional color distribution across images.

### Task 4: Texture and Color Matching
- **Objective:** Combine color histogram matching with Sobel magnitude histogram matching, equally weighted.
- **Method:** Histogram intersection is used to compare both color distribution and boundary texture between the target and query images.
- **Results:** For `pic.0535.jpg`, the results show a good balance of color and texture similarity, reflecting the equal weights given to color and texture metrics.

### Task 5: Custom Design - Sky Detection
- **Objective:** Rank images based on the presence of a blue sky using a custom reward/punish metric.
- **Method:** The algorithm extracts the upper half of each image, analyzes HSV values to detect blue sky, and applies a reward/punish system to rank images.
- **Results:** The method performs well with 100% accuracy for easy and medium sets, and 75% for the difficult set, with challenges arising from non-sky blue backgrounds.

## Extensions

### Extension A: Collage Creation
- **Objective:** Visualize the ranking results quickly by creating a collage of the top-ranked images.
- **Method:** The collage is automatically generated and saved as an output file.
- **Results:** A sample collage was created for Task 1, demonstrating the top 10 results from the Olympus dataset.

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

## How to Run

### Prerequisites
- **C++ Compiler** (e.g., GCC, MSVC)
- **OpenCV Library** (version 4.x or higher)
