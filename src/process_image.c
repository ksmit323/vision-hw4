#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    // Handle edge cases
    if (x < 0){
        x = 0;
    }else if (x >= im.w){
        x = im.w - 1;
    }
    if (y < 0){
        y = 0;
    }else if (y >= im.h){
        y = im.h - 1;
    }
    if (c < 0){
        c = 0;
    }else if (c >= im.c){
        c = im.c - 1;
    }

    // Find which channel, row and column the pixel is at in the data array
    int channel, row, col;
    channel = c * im.h * im.w;
    row = y * im.w;
    col = x;

    // Add these together to find the correct index in the data array
    float pixel = im.data[channel + row + col];

    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    // Handle edge cases
    if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c){
        printf("Coordinates out of bounds");
        return;
    }

    // Find the index in the data array (exactly the same way as get_pixel)
    int ind = c * im.h * im.w + y * im.w + x;

    // Set that value equal to v
    im.data[ind] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.h; i++){
        for (int j = 0; j < im.w; j++){
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);
            float v = 0.299*r + 0.587*g + 0.114*b;
            set_pixel(gray, j, i, 0, v);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++){
        for (int j = 0; j < im.h; j++){
            float new_v = v + get_pixel(im, i, j, c);
            set_pixel(im, i, j, c, new_v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    // Loop thru each pixel
    for (int i = 0; i < im.c; i++){
        for (int j = 0; j < im.w; j++){
            for (int k = 0; k < im.h; k++){
                float pixel = get_pixel(im, j, k, i);
                // Set the value to 1 if greater than one
                if (pixel > 1){
                    set_pixel(im, j, k, i, 1);
                // Set the value to 0 if less than zero
                }else if (pixel < 0){
                    set_pixel(im, j, k, i, 0);
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    // Loop thru the grid of pixels, starting with height and width
    for (int i = 0; i < im.w; i++){
        for (int j = 0; j < im.h; j++){

            // Find RGB values
            float R, G, B;
            R = get_pixel(im, i, j, 0);
            G = get_pixel(im, i, j, 1);
            B = get_pixel(im, i, j, 2);

            // Calculate Value and Saturation
            float V, m, C, S;

            // Value, V, is the max between RGB
            V = three_way_max(R, G, B);

            // Saturation is the ratio b/w the difference of the min and max and the Value
            m = three_way_min(R, G, B);
            C = V - m;
            if (V > 0){
                S = C / V;
            }else{
                S = 0;
            }

            // Calculate H
            float H = 0;
            if (C == 0){
                H = 0;
            }else if (V == R){
                H = (G-B) / C;
            }else if (V == G){
                H = ((B-R) / C) + 2;
            }else if (V == B){
                H = ((R-G) / C) + 4;
            }
            // Evaluate H
            if (H < 0){
                H = H/6 + 1;
            }else{
                H = H/6;
            }

            // Replace RGB values with HSV values
            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, S);
            set_pixel(im, i, j, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    // Loop thru the grid of pixels, starting with height and width
    for (int i = 0; i < im.w; i++){
        for (int j = 0; j < im.h; j++){

            // Find HSV values
            float H, S, V;
            H = get_pixel(im, i, j, 0);
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);

            // Calculate RGB values
            float R, G, B;
            float C = V * S;
            float H_prime = H*6;
            float X = C * (1 - fabs(fmod(H_prime, 2) - 1));
            float m = V - C;

            if (H_prime < 1){
                R = C;
                G = X;
                B = 0;
            }else if (H_prime < 2){
                R = X;
                G = C;
                B = 0;
            }else if (H_prime < 3){
                R = 0;
                G = C;
                B = X;
            }else if (H_prime < 4){
                R = 0;
                G = X;
                B = C;
            }else if (H_prime < 5){
                R = X;
                G = 0;
                B = C;
            }else if (H_prime < 6){
                R = C;
                G = 0;
                B = X;
            }else{
                R = 0;
                G = 0;
                B = 0;
            }

            // Replace HSV values with RGB values
            set_pixel(im, i, j, 0, R+m);
            set_pixel(im, i, j, 1, G+m);
            set_pixel(im, i, j, 2, B+m);
        }
    }
}
