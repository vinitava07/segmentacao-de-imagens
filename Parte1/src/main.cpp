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
    // escreve o header
    fprintf(saida, "%s\n%d %d\n%d\n", img->header.format, img->header.width, img->header.height, img->header.color);

    int comp;
    // pega a cor de cada componente
    for (int i = 0; i < img->header.height; i++)
    {
        for (int j = 0; j < img->header.width; j++)
        {
            comp = u->find((i * img->header.width + j));

            fprintf(saida, "%c%c%c", rImg[comp].red, rImg[comp].green, rImg[comp].blue);
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{

    char imageName[40];
    char fileName[40] = "";
    char prefix[30] = "images/";

    cout << "Qual o nome da imagem que você deseja segmentar (sem o .ppm)" << endl;
    cin >> imageName;
    strcpy(fileName, imageName);
    strcat(fileName, ".ppm");
    strcat(prefix, fileName);
    cout << prefix;
    Image *image = new Image(prefix);
    image->readImage();
    size_t graphSize = image->imgSize;

    int gauss;
    int threshold;
    int min;
    int grey;
    float sigma;

    cout << "Deseja transformar a imagem em preto e branco? (0) NAO - (1) SIM" << endl;
    cin >> grey;
    if (grey)
    {
        // transforma em escala de cinza
        image->greyScale();
    }
    cout << "Deseja utilizar uma mascara gaussiana? (0) NAO - (1) SIM" << endl;
    cin >> gauss;
    if (gauss)
    {
        cout << "Qual o valor do desvio padrao? (Padrao = 0.8)" << endl;
        cin >> sigma;
        // aplica filtro gaussiano
        image->smooth(sigma);
    }

    cout << "Qual o valor minimo para o tamanho dos componentes?" << endl;
    cin >> min;
    cout << "Qual o valor do threshold das arestas?" << endl;
    cin >> sigma;

    chrono::steady_clock sc; // create an object of `steady_clock` class
    auto start = sc.now();
    Graph *g = new Graph(graphSize, image, min, threshold);
    // criar o grafo relativo a imagem (pode ser melhorado)
    g->imageToGraph(image);
    // realiza a segmentação e devolve o union-find
    Universe *u = g->segmentation();

    // gerando cores aleatórias para a imagem segmentada
    Image::Pixel *p = (Image::Pixel *)malloc(g->edges->size() * sizeof(Image::Pixel));
    for (size_t i = 0; i < g->edges->size(); i++)
    {
        p[i].red = rand() % 255;
        p[i].green = rand() % 255;
        p[i].blue = rand() % 255;
    }
    writeImage(p, u, image, g);

    auto end = sc.now();                                                 // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<chrono::duration<double>>(end - start); // measure time span between start & end
    cout << "A segmentacao durou: " << time_span.count() << " segundos !!!" << endl;
    cout << "Lembre-se de excluir a saida.ppm para evitar erros ao gerar outro arquivo!";
    // system("pause");
    return 0;
}
