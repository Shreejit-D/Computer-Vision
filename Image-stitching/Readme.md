# Photo Mosaic Creation and Camera Calibration

## Introduction

This project aims to create photo mosaics using sets of images and estimate the camera error using the Caltech Camera Calibrator. The project is divided into two main sections:

1. **Phone Camera Error Estimation using Caltech Camera Calibrator**
2. **Photo Mosaic Creation using Different Sets of Images**

## Part I: Camera Calibration

### Objective
The objective of this section is to calibrate the camera using a planar checkerboard pattern and estimate the camera's distortion parameters.

### Method
- A checkerboard with individual square dimensions of 30mm x 30mm was used.
- Images of the checkerboard were captured from different angles and with varying focal lengths to account for distortions.
- The corners of the checkerboard were auto-detected, and calibration was performed to understand the camera calibration parameters and reprojection error.

### Results
- **Reprojection Error:** The reprojection error was found to be [0.76215, 0.99399] pixels.
- **Undistortion:** The calibration parameters were used to undistort images. The undistortion worked well in the center of the image but introduced new distortions along the borders.

### Visuals
- **Figure 1:** Images used for calibration, showcasing different angles and focal lengths.
- **Figure 2:** Calibration results.
- **Figure 3:** Reprojection error visualization.
- **Figure 4:** Visualization of reprojection for the first four images.
- **Figure 5:** Comparison between distorted and undistorted images.

### Conclusion
The distortion error estimated was less than 1, so calibrated images were not used for stitching. The intrinsic errors fluctuated significantly when camera parameters were changed, affecting the accuracy of the undistortion.

## Part II: Photo Mosaic Creation

### Objective
Create photo mosaics using different sets of images with varying overlaps and image characteristics.

### A. Latino Student Center
- **Images:** 8 images with approximately 50% overlap between consecutive images.
- **Method:** Harris corner detection was applied to detect important features, and the images were stitched to form a mosaic.
- **Results:** The stitching was successful with minimal errors due to the significant feature overlap between consecutive images.

### B. Cinder Block Wall
- **Images:** 8 images of a cinder block wall with Harris corners detected.
- **Method:** Initial stitching attempts resulted in a singular matrix error due to equally spaced corners. The resolution was reduced to decrease noise and improve feature mapping.
- **Results:** The stitching was less effective compared to the Latino Student Center due to the uniformity of the cinder block wall.

### C. Comparison Between 50% and 15% Overlap
- **Images:** A set of graffiti images with 50% overlap (8 images) and 15% overlap (5 images).
- **Method:** Harris corner detection was applied, and adjustments like resizing, cropping, and contrast enhancement were made to achieve better feature mapping for the 15% overlap set.
- **Results:** The 50% overlap mosaic was more accurate, while the 15% overlap mosaic required significant adjustments, leading to some loss of information.

### Visuals
- **Figure 8:** Latino building Center images with 50% overlap.
- **Figure 9:** Harris feature detection for individual images.
- **Figure 10:** Final stitched image of LSC building.
- **Figure 11:** Cinder block wall with Harris corners.
- **Figure 12:** Stitched cinder block image.
- **Figure 13:** 50% overlap graffiti images with Harris corners.
- **Figure 14:** 15% overlap graffiti images with Harris corners.
- **Figure 15:** Mosaic of 50% overlap graffiti images.
- **Figure 16:** Mosaic of 15% overlap graffiti images.

### Conclusion
- **Latino Student Center:** The high overlap and distinct features led to a successful mosaic.
- **Cinder Block Wall:** The uniform texture posed challenges, but reducing image resolution improved feature matching.
- **Graffiti Comparison:** The 50% overlap provided better results, while the 15% overlap required significant adjustments, leading to information loss.

### Code
The MATLAB code used for stitching all images is `image_stitching.m`, located in the analysis folder. The same parameters were used across all image sets.

## Conclusion

This project successfully demonstrated the creation of photo mosaics using different sets of images with varying overlaps and image characteristics. The camera calibration process highlighted the importance of accurate distortion parameters for undistorting images. The photo mosaic creation process showcased the effectiveness of feature detection and image stitching, with varying levels of success based on image overlap and texture.
