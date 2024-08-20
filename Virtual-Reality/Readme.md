# Camera Calibration and 3D Projection Using OpenCV

## Introduction

The aim of this project is to find the distortion coefficients caused by imperfections in the camera lens, the intrinsic camera matrix, and to use these values to transform 3D visualized objects back to a 2D image plane. The project is divided into 7 tasks, each focusing on different aspects of camera calibration and 3D projection.

## Tasks Overview

### Task 1: Finding the Inner Corners of the Checkerboard
- **Objective:** Identify the inner corners (9 columns and 6 rows) of a checkerboard using OpenCV functions.
- **Methods:**
  1. **findChessboardCorners:** Locates the internal corners of the chessboard, essential for camera calibration.
  2. **cornerSubPix:** Refines the corner positions to sub-pixel accuracy, improving the accuracy of subsequent image processing operations.
- **Flags Used:**
  - `CALIB_CB_ADAPTIVE_THRESH`: Enables adaptive thresholding for better corner detection under varying lighting conditions.
  - `CALIB_CB_NORMALIZE_IMAGE`: Normalizes the image to reduce the effects of noise.
  - `CALIB_CB_FAST_CHECK`: Speeds up the corner detection process by quickly checking for a chessboard pattern.

### Task 2: Drawing Circles on Detected Corners
- **Objective:** Highlight the detected inner corners by drawing circles on each point.
- **Visuals:** The corners detected in the calibration images are highlighted with circles.

### Task 3: Camera Calibration
- **Objective:** Use the `calibrateCamera` function in OpenCV to estimate the intrinsic and extrinsic parameters of the camera.
- **Output:** The function returns the calibrated camera matrix, distortion coefficients, and other parameters used for image rectification and 3D reconstruction.
- **Visuals:** Comparison of camera intrinsic matrix and distortion coefficients before and after calibration.

### Task 4: Estimating Camera Pose
- **Objective:** Estimate the camera's pose (rotation and translation) using the checkerboard pattern.
- **Method:** The `solvePNP` function is used to calculate the rotation and translation vectors. The flag `CALIB_FIX_ASPECT_RATIO` is applied to maintain the aspect ratio of the intrinsic matrix during calibration.

### Task 5: Projecting 3D Points onto 2D Image Plane
- **Objective:** Project 3D points onto a 2D image plane using the `projectPoints` function.
- **Method:** This function uses the camera matrix, rotation, and translation vectors to project 3D points (like the corners of the checkerboard) onto the 2D image.
- **Visuals:** Projection of 3D axes and corner points onto the checkerboard.

### Task 6: Projecting a 3D Object onto the Checkerboard
- **Objective:** Visualize a 3D object projected onto the 2D image plane of the checkerboard.
- **Method:** A star-shaped 3D object is created and projected onto the checkerboard image using the same technique as in Task 5.
- **Visuals:** The 3D star projected onto the checkerboard plane.

### Task 7: Harris Corner Detector
- **Objective:** Use the Harris corner detector to identify features in the image.
- **Method:** The Harris corner detector computes local intensity variations to detect corners, which are crucial for feature extraction and image alignment.
- **Application:** Tested on a custom design of dot positions, the detected corners are highlighted in the output images.
- **Extension:** In Augmented Reality, the detected features can be used to project images or videos onto a white background, enhancing the visual experience.

## Extension: Warping Pikachu onto a Cross 'X'
- **Objective:** Implement an extension to warp an image (Pikachu) onto a cross 'X' drawn on white paper.
- **Steps:**
  1. Convert the frame to grayscale for computational efficiency.
  2. Threshold the frame to create a binary image.
  3. Clean the binary image to remove noise.
  4. Detect the cross sign based on the number of contours.
  5. Apply a perspective transform to warp the Pikachu image onto the cross.
  6. Mask the cross to blend Pikachu with the background.
- **Functions Used:**
  - `getPerspectiveTransform`: Calculates the transformation matrix needed to warp the image.
  - `warpPerspective`: Applies the perspective transformation to align the image with the cross.
- **Visuals:** The final output showing Pikachu warped onto the cross 'X'.

## Conclusion

This project demonstrates the process of camera calibration, 3D projection, and feature detection using OpenCV. Each task builds on the previous one, resulting in a comprehensive understanding of camera calibration and the projection of 3D objects onto 2D image planes. The extension showcases the potential of applying these techniques in augmented reality.

---
