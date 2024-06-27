#include <iostream>
#include <string.h>
#include "../include/image.hpp"
#include <chrono>
#include "graph.hpp"
#include <random>

using namespace std;
void writeImage(Image::Pixel *rImg, Image *img, Graph *g)
{
    FILE *saida = fopen("saida.ppm", "wb+");
    if (!saida)
    {
        std::cerr << "Erro ao abrir o arquivo de saída!" << std::endl;
        return;
    }
    // escreve o header
    fprintf(saida, "%s\n%d %d\n%d\n", img->header.format, img->header.width, img->header.height, img->header.color);

    int comp;
    // pega a cor de cada componente
    for (int i = 0; i < img->header.height; i++)
    {
        for (int j = 0; j < img->header.width; j++)
        {
            comp = g->adj[i * img->header.width + j].seed;
            // if (comp == -1)
            // {
            //     fprintf(saida, "%c%c%c", 255, 255, 255);
            // }

            // // cout << comp <<" ";
            fprintf(saida, "%c%c%c", rImg[comp].red, rImg[comp].green, rImg[comp].blue);
            // fprintf(saida, "%c%c%c", img->img[i][j].red, img->img[i][j].green, img->img[i][j].blue);
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{
    chrono::steady_clock sc; // create an object of `steady_clock` class
    auto start = sc.now();
    Image *image = new Image("images/flamengo.ppm");
    image->readImage();
    image->greyScale();
    image->smooth(0.8);
    size_t graphSize = image->imgSize;
    Graph *g = new Graph(graphSize, image, 10, 5);
    g->imageToGraph(image);
    g->segmentation();
    Image::Pixel *p = (Image::Pixel *)malloc(g->nseeds * sizeof(Image::Pixel));
    for (size_t i = 0; i < g->nseeds; i++)
    {
        p[i].red = rand() % 256;
        p[i].green = rand() % 256;
        p[i].blue = rand() % 256;
    }
    writeImage(p, image, g);
    auto end = sc.now();                                                 // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<chrono::duration<double>>(end - start); // measure time span between start & end
    cout << "Operation took: " << time_span.count() << " seconds !!!";
    // system("pause");
    return 0;
}
