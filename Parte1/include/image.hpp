#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <string.h>
#include <string>


class Image
{
public:
    typedef struct
    {
        char format[3];
        int height;
        int width;
        int color;

    } ImageHeader;
    typedef struct
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;

    } Pixel;
    ImageHeader header;
    Pixel *pixels;
    Pixel **img;
    size_t imgSize;
    Image(const char *filename);
    ~Image();
    void readImage();
    Pixel **arrToMatrix();
    // void writeImage(Pixel *rImg, Universe *u);

private:
    FILE *f;
    void readFileUntil(char buffer[], char condition);
    void readImageHeader();
};

#endif