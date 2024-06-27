import numpy as np
import random

# Função para carregar a imagem PPM
def load_ppm_image(filename):
    with open(filename, 'rb') as f:
        header = f.readline().decode()
        assert header.strip() == 'P6'
        dimensions = f.readline().decode()
        width, height = map(int, dimensions.split())
        max_val = int(f.readline().decode().strip())
        img = np.fromfile(f, dtype=np.uint8).reshape((height, width, 3))
    return img

# Função para identificar diferentes elementos na imagem com limite de labels
def identify_elements_with_limit(image, label_limit=13):
    elements = {}
    label_counter = 1
    height, width, _ = image.shape
    for y in range(height):
        for x in range(width):
            color = tuple(image[y, x])
            if color not in elements:
                if label_counter <= label_limit:
                    elements[color] = label_counter
                    label_counter += 1
                else:
                    elements[color] = label_limit
    return elements

# Função para gerar seeds com coordenadas e labels
def generate_seeds(image, elements, num_seeds):
    height, width, _ = image.shape
    seeds = []
    seeds.append(f'{num_seeds} {width} {height}')
    for _ in range(num_seeds):
        x = random.randint(0, width - 1)
        y = random.randint(0, height - 1)
        color = tuple(image[y, x])
        label = elements[color]
        seeds.append(f'{x} {y} {label}')
    return seeds

# Carregar a imagem PPM
image = load_ppm_image('images/flamengo.ppm')

# Identificar diferentes elementos na imagem com limite de 13 labels
elements_limited = identify_elements_with_limit(image, label_limit=5000)

# Gerar 600 seeds
seeds_limited = generate_seeds(image, elements_limited, 10000)

# Salvar seeds em um arquivo
with open('seeds_flamengo.txt', 'w') as f:
    for seed in seeds_limited:
        f.write(f'{seed}\n')

"Arquivo de seeds com limite de labels gerado com sucesso!"