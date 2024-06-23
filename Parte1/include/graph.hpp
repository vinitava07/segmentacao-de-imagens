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

#define WIDTH 4
using namespace std;
class Graph
{
private:
    int height;
    int width;
    int min;
    int k;
    double sigma;

public:
    Vertex **adj;
    Vertex **resultGraph;
    std::vector<Edge> *edges;
    int vertices;
    void imageToGraph(Image *image)
    {
        int height = image->header.height;
        int width = image->header.width;
        int label = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                /* code */
                // cout << (int)image->img[i][j].red << " ";

                addPixel(i, j, image->img[i][j], label);
                label++;
                // addEdge(i, j, image.imgPixel[(i * height) + j].blue);
            }
            label++;
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                /* code */
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
                adj[i][j].check = true;
            }
        }

        free(image->img);
        delete image->pixels;
        for (int i = 0; i < image->header.height; i++)
        {
            delete[] adj[i]; // Libera cada array de Vertex
        }
        delete[] adj;
    }
    void addPixel(int i, int j, Image::Pixel pixel, int label)
    {

        adj[i][j].pixel = pixel;
        adj[i][j].label = label;
    }
    void addEdge(int i, int j, int destI, int destJ, int weight, Image::Pixel p)
    {
        if (!adj[destI][destJ].check)
        {
            int currLabel = adj[i][j].label;
            int destLabel = adj[destI][destJ].label;
            weight = (int)(intensity(p, adj[i][j].pixel));
            adj[i][j].addVertex(p, weight, destLabel);
            adj[destI][destJ].addVertex(adj[i][j].pixel, weight, currLabel);
            edges->push_back(Edge(new Vizinho(p, 0, currLabel), new Vizinho(adj[i][j].pixel, 0, destLabel), weight));
        }
    }
    float intensity(Image::Pixel p, Image::Pixel p2)
    {
        float result = 0;
        // result = (p.blue + p.green + p.red) / 3;
        result = std::sqrt(pow((p.red - p2.red), 2) + pow((p.green - p2.green), 2) + pow((p.blue - p2.blue), 2));
        // cout << result << " ";
        return result;
    }
    Universe *segmentation()
    {

        Universe *u = new Universe(edges->size(), vertices);

        float c = k;
        float *threshold = (float *)malloc(vertices * sizeof(float));
        for (int i = 0; i < vertices; i++)
        {
            getThreshold(1, c);
        }

        std::sort(edges->begin(), edges->end());
        for (int i = 0; i < edges->size(); i++)
        {

            int v1 = u->find(edges->at(i).v1->label);
            int v2 = u->find(edges->at(i).v2->label);
            // cout << "v1: " << v1 << " v2: " << v2 << endl;

            if (v1 != v2)
            {
                if (edges->at(i).value <= threshold[v1] && edges->at(i).value <= threshold[v2])
                {
                    u->join(v1, v2);
                    v1 = u->find(v1);
                    threshold[v1] = edges->at(i).value + getThreshold(u->size(v1), c);
                }
            }
        }

        for (int i = 0; i < edges->size(); i++)
        {
            int v1 = u->find(edges->at(i).v1->label);
            int v2 = u->find(edges->at(i).v2->label);
            if (v1 != v2 && u->size(v1) < min || u->size(v2) < min)
            {
                u->join(v1, v2);
            }
        }
        delete edges;
        return u;
    }
    float getThreshold(float size, float c)
    {
        return c / (size);
    }

    Image::Pixel **smooth(Image::Pixel **src)
    {
        std::vector<double> *mask = make_fgauss();
        mask = normalize(mask);
        Image::Pixel **tmp = convolve_even(src, mask);
        Image::Pixel **dst = convolve_even(tmp, mask);

        return dst;
    }

    // Função para criar máscara gaussiana
    std::vector<double> *make_fgauss()
    {
        sigma = std::max(sigma, 0.01);
        int length = std::ceil(sigma * WIDTH) + 1;
        std::vector<double> *mask = new std::vector<double>(length, 0.0);

        for (int i = 0; i < length; ++i)
        {
            mask->at(i) = std::exp(-0.5 * std::pow(i / sigma, i / sigma));
        }

        return mask;
    }

    // Função para normalizar a máscara
    std::vector<double> *normalize(const std::vector<double> *mask)
    {
        double sum = 2 * std::accumulate(mask->begin(), mask->end(), 0.0, [](double acc, double val)
                                         { return acc + std::abs(val); }) +
                     std::abs(mask->at(0));

        std::vector<double> *normalized_mask = new std::vector<double>(mask->size());
        std::transform(mask->begin(), mask->end(), normalized_mask->begin(), [sum](double val)
                       { return val / sum; });

        return normalized_mask;
    }

    // Função para convolver src com a máscara
    Image::Pixel **convolve_even(Image::Pixel **src, const std::vector<double> *mask)
    {
        int length = mask->size();
        Image::Pixel **output = new Image::Pixel *[height];
        for (int i = 0; i < height; ++i)
        {
            output[i] = new Image::Pixel[width];
        }

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                double sum_red = mask->at(0) * src[y][x].red;
                double sum_green = mask->at(0) * src[y][x].green;
                double sum_blue = mask->at(0) * src[y][x].blue;

                for (int i = 1; i < length; ++i)
                {
                    sum_red += mask->at(i) * (src[y][std::max(x - i, 0)].red + src[y][std::min(x + i, width - 1)].red);
                    sum_green += mask->at(i) * (src[y][std::max(x - i, 0)].green + src[y][std::min(x + i, width - 1)].green);
                    sum_blue += mask->at(i) * (src[y][std::max(x - i, 0)].blue + src[y][std::min(x + i, width - 1)].blue);
                }
                output[y][x].red = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_red)));
                output[y][x].green = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_green)));
                output[y][x].blue = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_blue)));
            }
        }

        return output;
    }
    Graph(size_t v, Image *img, int m, int ka, double sig)
    {
        min = m;
        k = ka;
        sigma = sigma;
        height = img->header.height;
        width = img->header.width;
        edges = new std::vector<Edge>();
        adj = new Vertex *[img->header.height];
        for (int i = 0; i < img->header.height; i++)
        {
            adj[i] = new Vertex[img->header.width];
        }

        vertices = v;
    };
    ~Graph()
    {
        free(adj);
    };
};
#endif