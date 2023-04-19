#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    // Find the nearest pixel
    int x_int = round(x);
    int y_int = round(y);

    // Get the pixel value
    float pixel = get_pixel(im, x_int, y_int, c);

    return pixel;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    // Use nearest neighbor to resize the image
    // Use the nn_interpolate function above to do so

    // Create new image
    image resized = make_image(w, h, im.c);

    // Map the new coordinate to the old coordinate following these steps for each pixel
    for (int y = 0; y < resized.h; y++){
        for (int x = 0; x < resized.w; x++){
            for (int c = 0; c < resized.c; c++){
                // The new pixel is proportional to the old: new_pixel/old_pixel
                // If the old image is 4x4 and the new one is 7x7 then 7 pixels
                // have to fit in the 4x4 space so the new pixel is 4/7th the size of the old one
                // These coordinates are set to (0,0) to start with but the actual coord of the pixel
                // we care about is in the center at (0.5, 0.5)
                // So we have to move the pixel to the center first, then scale it appropriately

                // Move the pixel to the center
                float old_x = x + 0.5;
                float old_y = y + 0.5;

                // Now scale the pixel according to the old image by multiplying it by the size ratio
                old_x = old_x * (im.w / (float)resized.w);
                old_y = old_y * (im.h / (float)resized.h);

                // Move the pixel back to the point of origin at (0,0)
                old_x -= 0.5;
                old_y -= 0.5;

                // Now that we have the old coordinates, interpolate from that position
                float pixel = nn_interpolate(im, old_x, old_y, c);

                // Finally, set the new pixel equal to this interpolation
                set_pixel(resized, x, y, c, pixel);
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    // Use bilinear interpolation to find the pixel value.
    // Find q1 and q2 for using linear interpolation of linear interpolates
    // q equals interpolation of q1 and q2

    float q1, q2, q, v1, v2, v3, v4, d1, d2, d3, d4;

    // Find the four corner coordinates nearest to the coordinate (x,y)
    float floor_x = floor(x);
    float floor_y = floor(y);
    float ceiling_x = ceil(x);
    float ceiling_y = ceil(y);

    // Now get the pixel value at each corner
    v1 = get_pixel(im, floor_x, floor_y, c);
    v2 = get_pixel(im, ceiling_x, floor_y, c);
    v3 = get_pixel(im, floor_x, ceiling_y, c);
    v4 = get_pixel(im, ceiling_x, ceiling_y, c);

    // Find the distance on the x axis between the coordinate and the corner coordinates
    d1 = x - floor_x;
    d2 = ceiling_x - x;

    // Now find q1 and q2 by interpolating
    q1 = v1 * d2 + v2 * d1;
    q2 = v3 * d2 + v4 * d1;

    // Find the distance on the y axis between the coordinates
    d3 = y - floor_y;
    d4 = ceiling_y - y;

    // Find q by interpolating between q1 and q2
    q = q1 * d4 + q2 * d3;

    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO

    // Create new image
    image resized = make_image(w, h, im.c);

    // Map the new coordinate to the old coordinate following these steps for each pixel
    for (int y = 0; y < resized.h; y++){
        for (int x = 0; x < resized.w; x++){
            for (int c = 0; c < resized.c; c++){

                // Move the coordinates to the center of the pixel
                float old_x = x + 0.5;
                float old_y = y + 0.5;

                // Now scale the pixel according to the old image by multiplying it by the size ratio
                old_x = old_x * (im.w / (float)resized.w);
                old_y = old_y * (im.h / (float)resized.h);

                // Move the pixel back to the point of origin
                old_x -= 0.5;
                old_y -= 0.5;

                // Now that we have the old coordinates, interpolate from that position
                float pixel = bilinear_interpolate(im, old_x, old_y, c);

                // Finally, set the new pixel equal to this interpolation
                set_pixel(resized, x, y, c, pixel);
            }
        }
    }

    return resized;
}