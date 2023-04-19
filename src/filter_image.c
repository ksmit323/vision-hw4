#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    // Normalize the image so that the sum of all the pixels sum to 1

    // Find the total pixel sum across all pixels
    float pixel_sum = 0;
    for (int i = 0; i < im.h*im.w*im.c; i++){
        pixel_sum += im.data[i];
    }

    // Divide each pixel by the pixel sum
    for (int i = 0; i < im.h*im.w*im.c; i++){
        im.data[i] /= pixel_sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    // Create a new image
    image box_filter = make_image(w, w, 1);

    // Fill in the new image with all 1's
    for (int i = 0; i < w*w; i++){
        box_filter.data[i] = 1;
    }

    l1_normalize(box_filter);

    return box_filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO

    //Filter better have the same number of channels as the input image or have 1
    assert(filter.c == im.c || filter.c == 1);

    // if preserve is set to 1, the output image will have the same number of channels as the input image
    int channels = (preserve) ? im.c : 1;


    // Create new image
    image convolved = make_image(im.w, im.h, channels);
    float convolved_pixel;

    // Loop through each pixel in the image
    for (int i = 0; i < im.h; i++){
        for (int j = 0; j < im.w; j++){

            if (!preserve){
                convolved_pixel = 0;
            }

            for(int k = 0; k < im.c; k++){

                if (preserve){
                    convolved_pixel = 0;
                }

                // Loop through each kernel
                for (int l = 0; l < filter.h; l++){
                    for (int m = 0; m < filter.w; m++){

                        // Set the coordinates to get the pixel value in the original image
                        int x = j - filter.w/2 + m;
                        int y = i - filter.h/2 + l;

                        // Get the kernel value in the filter image
                        float kernel;
                        if (filter.c == 1){
                            kernel = get_pixel(filter, m, l, 0);
                        } else {
                            kernel = get_pixel(filter, m, l, k);
                        }

                        // Multiply the pixel value by the kernel value and add to the convolved pixel
                        float original_pixel = get_pixel(im, x, y, k);
                        convolved_pixel += original_pixel * kernel;
                    }
                }

                // Set the convolved pixel
                if (preserve){
                    set_pixel(convolved, j, i, k, convolved_pixel);
                } else {
                    set_pixel(convolved, j, i, 0, convolved_pixel);
                }
            }
        }
    }

    return convolved;
}

image make_highpass_filter()
{
    // TODO
    image highpass = make_image(3,3,1);
    highpass.data[0] = 0;
    highpass.data[1] = -1;
    highpass.data[2] = 0;
    highpass.data[3] = -1;
    highpass.data[4] = 4;
    highpass.data[5] = -1;
    highpass.data[6] = 0;
    highpass.data[7] = -1;
    highpass.data[8] = 0;
    return highpass;
}

image make_sharpen_filter()
{
    // TODO
    image sharpen = make_image(3,3,1);
    sharpen.data[0] = 0;
    sharpen.data[1] = -1;
    sharpen.data[2] = 0;
    sharpen.data[3] = -1;
    sharpen.data[4] = 5;
    sharpen.data[5] = -1;
    sharpen.data[6] = 0;
    sharpen.data[7] = -1;
    sharpen.data[8] = 0;
    return sharpen;
}

image make_emboss_filter()
{
    // TODO
    image emboss = make_image(3,3,1);
    emboss.data[0] = -2;
    emboss.data[1] = -1;
    emboss.data[2] = 0;
    emboss.data[3] = -1;
    emboss.data[4] = 1;
    emboss.data[5] = 1;
    emboss.data[6] = 0;
    emboss.data[7] = 1;
    emboss.data[8] = 2;
    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    // Instructions state to make the filter/kernel size 6 times bigger than sigma
    int kernel_size = sigma * 6;

    // We want the dimensions to be an odd number
    if (kernel_size % 2 == 0){
        kernel_size += 1;
    }

    // Create a new image
    image gauss = make_image(kernel_size, kernel_size, 1);

    // Fill in the kernel with gaussian values
    for (int i = 0; i < gauss.h; i++){
        for (int j = 0; j < gauss.w; j++){

            // Set the center coordinates for x and y
            int x  = j - gauss.w/2;
            int y  = i - gauss.h/2;

            // Use the formula given in the slides
            float part_1 = 1 / (TWOPI * sigma*sigma);
            float part_2 = exp(-(x*x + y*y)/(2*sigma*sigma));
            float gaussian = part_1 * part_2;
            set_pixel(gauss, j, i, 0, gaussian);
        }
    }

    l1_normalize(gauss);

    return gauss;
}

image add_image(image a, image b)
{
    // TODO
    // Check that both images are the same size
    int a_size = a.h * a.w * a.c;
    int b_size = b.h * b.w * b.c;
    assert(a_size == b_size);

    // Create new image
    image sum = make_image(a.w, a.h, a.c);

    // Add both images together by adding the pixels together
    for (int i = 0; i < a_size; i++){
        sum.data[i] = a.data[i] + b.data[i];
    }

    return sum;
}

image sub_image(image a, image b)
{
    // TODO
    // Check that both images are the same size
    int a_size = a.h * a.w * a.c;
    int b_size = b.h * b.w * b.c;
    assert(a_size == b_size);

    // Create new image
    image sub = make_image(a.w, a.h, a.c);

    // Subtract both images together by subtracting the pixels 
    for (int i = 0; i < a_size; i++){
        sub.data[i] = a.data[i] - b.data[i];
    }

    return sub;
}

image make_gx_filter()
{
    // TODO
    image gx = make_image(3,3,1);
    gx.data[0] = -1;
    gx.data[1] = 0;
    gx.data[2] = 1;
    gx.data[3] = -2;
    gx.data[4] = 0;
    gx.data[5] = 2;
    gx.data[6] = -1;
    gx.data[7] = 0;
    gx.data[8] = 1;
    return gx;
}

image make_gy_filter()
{
    // TODO
    image gy = make_image(3,3,1);
    gy.data[0] = -1;
    gy.data[1] = -2;
    gy.data[2] = -1;
    gy.data[3] = 0;
    gy.data[4] = 0;
    gy.data[5] = 0;
    gy.data[6] = 1;
    gy.data[7] = 2;
    gy.data[8] = 1;
    return gy;
}

void feature_normalize(image im)
{
    // TODO
    // Find min, max values from the image
    float min = INFINITY;
    float max = -1.0;

    for (int i = 0; i < im.w * im.h * im.c; i++){
        if (im.data[i] < min){
            min = im.data[i];
        }

        if (im.data[i] > max){
            max = im.data[i];
        }
    }

    // Find the range
    float range = max - min;

    // Normalize each pixel to be between values 0 and 1
    for (int i = 0; i < im.w * im.h * im.c; i++){
        if (range == 0){
            im.data[i] = 0;
        }
        else{
            im.data[i] = (im.data[i] - min) / range;
        }
    }
}

image *sobel_image(image im)
{
    // TODO

    // Create an array of image pointers
    image *result_image = calloc(2, sizeof(image));

    // Convolve the image with the sobel filters
    image GX = convolve_image(im, make_gx_filter(), 0);
    image GY = convolve_image(im, make_gy_filter(), 0);

    // Create the magnitude and direction images
    result_image[0] = make_image(GX.w, GX.h, GX.c);
    result_image[1] = make_image(GX.w, GX.h, GX.c);

    // Calculate the magnitude and direction of the image
    for (int i = 0; i < GX.w * GX.h * GX.c; i++){
        // Magnitude
        result_image[0].data[i] = sqrt(GX.data[i]*GX.data[i] + GY.data[i]*GY.data[i]);
        // Direction
        result_image[1].data[i] = atan2(GY.data[i], GX.data[i]);
    }

    return result_image;
}

image colorize_sobel(image im)
{
    // TODO
    // Get the magnitude and direction images
    image *sobel = sobel_image(im);
    image magnitude = sobel[0];
    image direction = sobel[1];
    feature_normalize(magnitude);

    // Create a new image to hold the colorized sobel image
    image color = make_image(im.w, im.h, 3);

    // Set the pixels of the color image
    for (int c = 0; c < im.c; c++){
        for (int y = 0; y < im.h; y++){
            for(int x = 0; x < im.w; x++){

                // Set all Hue pixels to the direction
                if (c == 0){
                    float hue_pixel = get_pixel(direction, x, y, c);
                    set_pixel(color, x, y, c, hue_pixel);
                }else{
                // Set all Saturation and Value pixels to the magnitude
                    float pixel = get_pixel(magnitude, x, y, c);
                    set_pixel(color, x, y, 1, pixel);
                    set_pixel(color, x, y, 2, pixel);
                }
            }
        }
    }

    // Convert the color image to HSV
    hsv_to_rgb(color);

    // Free the sobel image
    free_image(magnitude);
    free_image(direction);

    return color;
}

