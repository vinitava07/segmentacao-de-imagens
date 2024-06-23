#include <iostream>
#include <string.h>
#include "../include/image.hpp"
#include <chrono>
#include "graph.hpp"
#include <random>

using namespace std;
void writeImage(Image::Pixel *rImg, Universe *u, Image *img, Graph *g)
{
    FILE *saida = fopen("saida.ppm", "wb+");
    if (!saida)
    {
        std::cerr << "Erro ao abrir o arquivo de saída!" << std::endl;
        return;
    }

    fprintf(saida, "%s\n%d %d\n%d\n", img->header.format, img->header.width, img->header.height, img->header.color);

    int comp;
    for (int i = 0; i < img->header.height; i++)
    {
        for (int j = 0; j < img->header.width; j++)
        {
            comp = u->find((i * img->header.width + j) + (i));

            fprintf(saida, "%c%c%c", rImg[comp].red, rImg[comp].green, rImg[comp].blue);
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{
    chrono::steady_clock sc; // create an object of `steady_clock` class
    auto start = sc.now();
    Image *image = new Image("greyFlowers.ppm");
    image->readImage();
    size_t graphSize = image->imgSize;
    int count = 0;
    Graph *g = new Graph(graphSize, image, 50, 5000, 0.8);
    image->img = g->smooth(image->img);
    g->imageToGraph(image);
    Universe *u = g->segmentation();
    Image::Pixel *p = (Image::Pixel *)malloc(g->edges->size() * sizeof(Image::Pixel));
    for (size_t i = 0; i < g->edges->size(); i++)
    {
        p[i].red = rand() % 255;
        p[i].green = rand() % 255;
        p[i].blue = rand() % 255;
    }
    writeImage(p, u, image, g);
    // delete g;

    auto end = sc.now();                                                 // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<chrono::duration<double>>(end - start); // measure time span between start & end
    cout << "Operation took: " << time_span.count() << " seconds !!!";
    system("pause");
    return 0;
}
