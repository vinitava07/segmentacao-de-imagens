# Segmentação de Imagens Baseada em Grafos

Este repositório contém a implementação de dois algoritmos de segmentação de imagens baseados em grafos:

1. **Segmentação de Imagens Eficiente Baseada em Grafos** por Felzenszwalb e Huttenlocher (2004)
2. **The image foresting transform** por Falcao, Stolfi e Lotufo (2004)

## Algoritmos

### 1. Segmentação de Imagens Eficiente Baseada em Grafos

Felzenszwalb, P. F. e Huttenlocher, D. P. (2004). "Efficient graph-based image segmentation". *International Journal of Computer Vision*.

Este algoritmo é projetado para segmentar imagens de forma eficiente, representando-as como grafos e encontrando regiões coerentes em termos de propriedades visuais.

### 2. IFT - The image foresting transform

Falcao, A. X., Stolfi, J., e Lotufo, R. d. A. (2004). "The image foresting transform: theory, algorithms, and applications". *IEEE Transactions on Pattern Analysis and Machine Intelligence*, 26(1):19–29.

Este algoritmo segmenta imagens transformando-as em uma floresta, onde as raízes das árvores são determinadas pelo usuário.

## Como Executar

Para rodar o código de cada algoritmo, siga as instruções abaixo:

### Segmentação de Imagens Eficiente Baseada em Grafos

1. Navegue até o diretório `Parte1`:
    ```sh
    cd Parte1
    ```
2. Execute o Makefile:
    ```sh
    make
    ```
3. Execute o programa principal:
    ```sh
    ./main
    ```

### IFT

1. Navegue até o diretório `Parte2`:
    ```sh
    cd Parte2
    ```
2. Execute o Makefile:
    ```sh
    make
    ```
3. Execute o programa principal:
    ```sh
    ./main
    ```

## Dependências

Certifique-se de ter as seguintes dependências instaladas:

- Compilador C++ (e.g., `g++`)
- Make

Você pode instalar as dependências necessárias usando um gerenciador de pacotes como o `apt` para sistemas baseados em Debian:

```sh
sudo apt-get update
sudo apt-get install g++ make
```
Sinta-se à vontade para contribuir com este repositório enviando issues ou pull requests. Para qualquer dúvida, por favor, entre em contato com o proprietário do repositório.
