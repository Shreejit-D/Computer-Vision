# Photo Mosaic Creation and Camera Calibration

## Introduction

This project aims to create photo mosaics using sets of images and estimate the camera error using the Caltech Camera Calibrator. The project is divided into two main sections:

1. **Phone Camera Error Estimation using Caltech Camera Calibrator**
2. **Photo Mosaic Creation using Different Sets of Images**

## Part I: Camera Calibration

### Objective
The objective of this section is to calibrate the camera using a planar checkerboard pattern and estimate the camera's distortion parameters.

![image](https://github.com/user-attachments/assets/b01958c5-b531-4879-b655-2b266d7daeb8)


### Method
- A checkerboard with individual square dimensions of 30mm x 30mm was used.
- Images of the checkerboard were captured from different angles and with varying focal lengths to account for distortions.
- The corners of the checkerboard were auto-detected, and calibration was performed to understand the camera calibration parameters and reprojection error.

### Results
- **Reprojection Error:** The reprojection error was found to be [0.76215, 0.99399] pixels.
- **Undistortion:** The calibration parameters were used to undistort images. The undistortion worked well in the center of the image but introduced new distortions along the borders.

![image](https://github.com/user-attachments/assets/356ded27-de89-438e-9020-5f84fee60172)


![image](https://github.com/user-attachments/assets/2d7ff23e-0249-43a1-8b12-68330e50d5e5)


## Part II: Photo Mosaic Creation

### Objective
Create photo mosaics using different sets of images with varying overlaps and image characteristics.

### A. Latino Student Center
- **Images:** 8 images with approximately 50% overlap between consecutive images.
- **Method:** Harris corner detection was applied to detect important features, and the images were stitched to form a mosaic.
- **Results:** The stitching was successful with minimal errors due to the significant feature overlap between consecutive images.


![image](https://github.com/user-attachments/assets/ff19e06c-cd0c-462c-8b60-660056252b75)

![image](https://github.com/user-attachments/assets/584222df-007e-4ee2-8189-9c334a068972)

### B. Cinder Block Wall
- **Images:** 8 images of a cinder block wall with Harris corners detected.
- **Method:** Initial stitching attempts resulted in a singular matrix error due to equally spaced corners. The resolution was reduced to decrease noise and improve feature mapping.
- **Results:** The stitching was less effective compared to the Latino Student Center due to the uniformity of the cinder block wall.

![image](https://github.com/user-attachments/assets/d51fc817-3a5c-46d0-b114-70a8f8a04921)

### C. Comparison Between 50% and 15% Overlap
- **Images:** A set of graffiti images with 50% overlap (8 images) and 15% overlap (5 images).
- **Method:** Harris corner detection was applied, and adjustments like resizing, cropping, and contrast enhancement were made to achieve better feature mapping for the 15% overlap set.
- **Results:** The 50% overlap mosaic was more accurate, while the 15% overlap mosaic required significant adjustments, leading to some loss of information.

![image](https://github.com/user-attachments/assets/fac3118a-d327-46bd-b2c7-82aa7bdbc00e)

### Conclusion
- **Latino Student Center:** The high overlap and distinct features led to a successful mosaic.
- **Cinder Block Wall:** The uniform texture posed challenges, but reducing image resolution improved feature matching.
- **Graffiti Comparison:** The 50% overlap provided better results, while the 15% overlap required significant adjustments, leading to information loss.

### Code
The MATLAB code used for stitching all images is `image_stitching.m`. The same parameters were used across all image sets.

## Conclusion

This project successfully demonstrated the creation of photo mosaics using different sets of images with varying overlaps and image characteristics. The camera calibration process highlighted the importance of accurate distortion parameters for undistorting images. The photo mosaic creation process showcased the effectiveness of feature detection and image stitching, with varying levels of success based on image overlap and texture.
