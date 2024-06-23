
#include "image.hpp"
#include <iostream>
#include <string.h>
#include <unionFind.hpp>

Image::Image(const char *filename)
{
    header = {
        .height = 0,
        .width = 0,
        .color = 0};
    f = fopen(filename, "rb+");
    if (f == NULL)
    {
        printf("The file is not opened. The program will now exit.");
        exit(0);
    }
    readImageHeader();
    imgSize = header.height * header.width;
    pixels = (Pixel *)malloc(sizeof(Pixel) * imgSize);
};
Image::~Image()
{
    fclose(f);
    free(pixels);
};
void Image::readImage()
{
    size_t triples = (imgSize * 3);
    unsigned char *buffer = (unsigned char *)malloc(sizeof(char) * triples + 1);
    fread(buffer, sizeof(char), triples, f);
    size_t bufferOffset = 0;
    for (size_t i = 0; i < imgSize; i++)
    {
        pixels[i].red = (int)buffer[bufferOffset];
        pixels[i].green = (int)buffer[bufferOffset + 1];
        pixels[i].blue = (int)buffer[bufferOffset + 2];
        bufferOffset += 3;
    }
    free(buffer);
    arrToMatrix();
}
Image::Pixel **Image::arrToMatrix()
{
    img = new Pixel *[header.height];
    for (int i = 0; i < header.height; i++)
    {
        img[i] = new Pixel[header.width];
    }

    // Transforma o vetor de pixels na matriz de pixels
    for (int i = 0; i < header.height; i++)
    {
        for (int j = 0; j < header.width; j++)
        {
            img[i][j] = pixels[i * header.width + j];
        }
    }

    return img;
}

void Image::readFileUntil(char buffer[], char condition)
{
    char c;
    int cont = 0;
    do
    {
        c = fgetc(f);
        buffer[cont++] = c;
    } while (c != condition);
    buffer[cont - 1] = '\0';
}

void Image::readImageHeader()
{
    char format[3];
    char fHeight[5];
    char fWidth[5];
    char colorScheme[4];
    readFileUntil(format, '\n');
    readFileUntil(fWidth, ' ');
    readFileUntil(fHeight, '\n');
    readFileUntil(colorScheme, '\n');
    header.color = atoi(colorScheme);
    strcpy(header.format, format);
    header.height = atoi(fHeight);
    header.width = atoi(fWidth);
}
