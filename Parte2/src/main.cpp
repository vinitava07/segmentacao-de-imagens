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
            fprintf(saida, "%c%c%c", rImg[comp].red, rImg[comp].green, rImg[comp].blue);
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{
    chrono::steady_clock sc; // create an object of `steady_clock` class
    Image *image = new Image("images/billgreen.ppm");
    int grey = 0;
    int gauss = 1;
    int limit = 1;
    float sigma = 0.8;
    float threshold = 1;
    image->readImage();
    cout << "Deseja transformar a imagem em preto e branco? (0) NAO - (1) SIM" << endl;
    // cin >> grey;
    if (grey)
    {
        // transforma em escala de cinza
        image->greyScale();
    }
    cout << "Deseja utilizar uma mascara gaussiana? (0) NAO - (1) SIM" << endl;
    // cin >> gauss;
    if (gauss)
    {
        cout << "Qual o valor do desvio padrao? (Padrao = 0.8)" << endl;
        // cin >> sigma;
        // aplica filtro gaussiano
        image->smooth(sigma);
    }
    cout << "Deseja permitir um limiar de caminhamento? (0) NAO - (1) SIM" << endl;
    // cin >> limit;
    if (limit)
    {
        cout << "Qual o valor do limiar? (Padrao = 10000)" << endl;
        // cin >> threshold;
    }

    auto start = sc.now();
    size_t graphSize = image->imgSize;
    Graph *g = new Graph(graphSize, image, threshold);
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
