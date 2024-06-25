#ifndef UNGRAPH_HPP
#define UNGRAPH_HPP

#include <vertex.hpp>
#include <vector>
#include <iostream>
#include <image.hpp>
#include <algorithm>
#include <iterator>
#include <unionFind.hpp>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>

#define WIDTH 4
using namespace std;
class Graph
{
private:
    int height;
    int width;
    int min;
    int k;
    typedef struct
    {
        int x, y, mk, la;
    } Seed;

public:
    Vertex *adj;
    Vertex **resultGraph;
    Seed *seeds;
    int nseeds;
    std::vector<Edge> *edges;
    int vertices;
    void imageToGraph(Image *image)
    {
        int height = image->header.height;
        int width = image->header.width;
        int label = 0;
        readSeed();
        // estebelece os pixeis para cada vertice
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                addPixel(i, j, image->img[i][j], label);
                label++;
            }
        }
        cout << nseeds << endl;
        for (int i = 0; i < nseeds; i++)
        {
            int x = seeds[i].x;
            int y = seeds[i].y;
            int mk = seeds[i].mk;
            int la = seeds[i].la;

            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                adj[y * width + x].weight = 0;
                adj[y * width + x].seed = mk;
            }
        }
        // liga os vertices pela vizinhaça dos 8 lados na imagem
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i + 1 < height && j + 1 < width)
                {
                    addEdge(i, j, i + 1, j + 1, 0, image->img[i + 1][j + 1]);
                }
                if (j + 1 < width)
                {
                    addEdge(i, j, i, j + 1, 0, image->img[i][j + 1]);
                }
                if (i + 1 < height)
                {
                    addEdge(i, j, i + 1, j, 0, image->img[i + 1][j]);
                }
                if (i + 1 < height && j - 1 >= 0)
                {
                    addEdge(i, j, i + 1, j - 1, 0, image->img[i + 1][j - 1]);
                }
                if (j - 1 >= 0)
                {
                    addEdge(i, j, i, j - 1, 0, image->img[i][j - 1]);
                }
                if (i - 1 >= 0)
                {
                    addEdge(i, j, i - 1, j, 0, image->img[i - 1][j]);
                }
                if (i - 1 >= 0 && j + 1 < width)
                {
                    addEdge(i, j, i - 1, j + 1, 0, image->img[i - 1][j + 1]);
                }
                if (i - 1 >= 0 && j - 1 >= 0)
                {
                    addEdge(i, j, i - 1, j - 1, 0, image->img[i - 1][j - 1]);
                }
                adj[i * width + j].check = true;
            }
        }

        free(image->img);
        delete image->pixels;
        // for (int i = 0; i < image->header.height; i++)
        // {
        //     delete[] adj[i]; // Libera cada array de Vertex
        // }
        // delete[] adj;
    }
    void readSeed()
    {
        FILE *fp = fopen("seeds.txt", "r");
        int ncols, nrows;

        if (fp == NULL)
        {
            fprintf(stderr, "Erro ao abrir o arquivo seeds.txt\n");
            return;
        }
        if (fscanf(fp, "%d %d %d", &nseeds, &ncols, &nrows) != 3)
        {
            fprintf(stderr, "Formato inválido no arquivo seeds.txt\n");
            fclose(fp);
            return;
        }
        seeds = (Seed *)malloc(nseeds * sizeof(Seed));
        for (int i = 0; i < nseeds; i++)
        {
            if (fscanf(fp, "%d %d %d %d", &seeds[i].x, &seeds[i].y, &seeds[i].mk, &seeds[i].la) != 4)
            {
                fprintf(stderr, "Formato inválido no arquivo seeds.txt\n");
                free(seeds);
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }
    void searchSeed(Seed *seeds, int index, int x, int y, int mk, int la)
    {

        seeds[index].x = x;
        seeds[index].y = y;
        seeds[index].mk = mk;
        seeds[index].la = la;
    }
    void addPixel(int i, int j, Image::Pixel pixel, int label)
    {
        adj[i * width + j].pixel = pixel;
        adj[i * width + j].label = label;
        adj[i * width + j].weight = INT32_MAX;
    }
    void addEdge(int i, int j, int destI, int destJ, int weight, Image::Pixel p)
    {
        if (!adj[destI * width + destJ].check)
        {
            int currLabel = adj[i * width + j].label;
            int destLabel = adj[destI * width + destJ].label;
            int currW = adj[i * width + j].weight;
            int destW = adj[destI * width + destJ].weight;
            int w3 = (adj[destI * width + destJ].pixel.red + adj[destI * width + destJ].pixel.green + adj[destI * width + destJ].pixel.blue) / 3;

            weight = (int)(intensity(p, adj[i * width + j].pixel));
            // cout << "aresta: " << weight << endl;

            adj[i * width + j].addVertex(p, weight, destLabel, destW);
            adj[destI * width + destJ].addVertex(adj[i * width + j].pixel, weight, currLabel, currW);
            // edges->push_back(Edge(new Vizinho(p, 0, currLabel), new Vizinho(adj[i * width + j].pixel, 0, destLabel), weight));
        }
    }
    // equilibrar o peso da aresta baseado nos pixeis que ela liga
    float intensity(Image::Pixel p, Image::Pixel p2)
    {
        float result = 0;
        // result = std::abs(((p.red + p.green + p.blue) / 3) - ((p2.red + p2.green + p2.blue) / 3));
        result = std::sqrt(pow((p.red - p2.red), 2) + pow((p.green - p2.green), 2) + pow((p.blue - p2.blue), 2));
        return result;
    }

    void segmentation()
    {
        std::priority_queue<Vertex *, std::vector<Vertex *>, Vertex::CompareVertex> *queue = new std::priority_queue<Vertex *, std::vector<Vertex *>, Vertex::CompareVertex>();
        std::vector<int> *visited = new std::vector<int>();
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (adj[i * width + j].weight != INT32_MAX)
                {
                    queue->emplace(&adj[i * width + j]);
                    if (adj[i * width + j].seed != 0 && adj[i * width + j].seed != 1)
                    {
                        /* code */
                        // cout << "elemento RETIRADOOO: " << adj[i * width + j].seed << endl;
                    }
                    // cout << "elemento adicionado: " << adj[i * width + j].label << endl;
                }
            }
        }
        Vertex *v;
        int count = 0;
        while (!queue->empty())
        {
            v = queue->top();
            // if (v->weight == 0 )
            // {
            if (v->seed != 0 && v->seed != 1)
            {
                /* code */
                // cout << "elemento RETIRADOOO: " << v->seed << endl;
            }

            //     /* code */
            //     count++;
            //     continue;
            // }
            queue->pop(); // Remover o nó da fila de prioridades

            for (int i = 0; i < (v->adj->size()); i++)
            {
                int pathCost = (v->adj->at(i).edge);
                // cout << "peso da aresta: " << pathCost << endl;

                // int novoCusto = pathCost + newCost(v->pixel, v->adj->at(i).p);
                // int limit = (int)(intensity(v->pixel, adj[v->adj->at(i).label].pixel));
                if (adj[v->adj->at(i).label].weight > v->weight + pathCost)
                {
                    // Atualizar o peso do nó adjacente
                    if (adj[v->adj->at(i).label].seed != -1 && pathCost > 5)
                    {
                        /* code */
                    }
                    else
                    {

                        v->adj->at(i).weight = v->weight + pathCost;
                        adj[v->adj->at(i).label].weight = v->weight + pathCost;
                        adj[v->adj->at(i).label].seed = v->seed;
                        // cout << "seed vencedora: " << v->seed << endl;
                        queue->emplace(&adj[v->adj->at(i).label]);
                        // cout << "elemento adicionado: " << v->adj->at(i).label << endl;
                        // cout << "elemento peso: " << adj[v->adj->at(i).label].weight << endl;
                    }
                }
            }
            count++;
        }
    }
    int newCost(Image::Pixel p1, Image::Pixel p2)
    {
        int result = std::abs(((p1.red + p1.green + p1.blue) / 3) - ((p2.red + p2.green + p2.blue) / 3));
        return 0;
    }
    bool isVisited(std::vector<int> *v, int label)
    {
        for (int i = 0; i < v->size(); i++)
        {
            if (v->at(i) == label)
            {
                return true;
            }
        }
        return false;
    }

    Graph(size_t v, Image *img, int m, int ka)
    {
        min = m;
        k = ka;
        height = img->header.height;
        width = img->header.width;
        // edges = new std::vector<Edge>();
        adj = new Vertex[img->header.height * img->header.width];

        vertices = v;
    };
    ~Graph()
    {
        free(adj);
    };
};
#endif