from ursina import *
from ursina.shaders import basic_lighting_shader
from PIL import Image
from noise import Noise2D
import numpy as np

def heightCurve(x):
    if x < 0:
        return 0
    elif x > 1:
        return 1
    else:
        return x ** 2

def falloff(x):
    a = 3
    b=0.1
    if x < 0:
        return 0
    elif x > 1:
        return 1
    else:
        return (x ** a) / ((x ** a) + (b - b * x) **a)

# noise parameters
seed = 12782
size = 512
scale = 160
octave = 4
lacunarity = 2.0
persistence = 0.5

# generate noise
noiseObj = Noise2D(seed, size, size, scale, octave, lacunarity, persistence)
noiseData = [[int(heightCurve(noiseObj[i, j] + 0.5) * falloff(min(i, j, size - i, size - j) / size) * 200.0) for j in range(size)] for i in range(size)]

# deal with out of range (0~255)
for i in range(size):
    for j in range(size):
        if noiseData[i][j] < 0:
            noiseData[i][j] = 0
        elif noiseData[i][j] > 255:
            noiseData[i][j] = 255

# generate height map
heightMap = Image.fromarray(np.array(noiseData, np.uint8))
heightMap.save('render/heightmap.png')

# transform height map data to color map
for i in range(size):
    for j in range(size):
        if noiseData[i][j] > 200:
            noiseData[i][j] = (250, 240, 230)
        elif noiseData[i][j] > 150:
            noiseData[i][j] = (77, 31, 0)
        elif noiseData[i][j] > 80:
            noiseData[i][j] = (0, 128, 0)
        elif noiseData[i][j] > 40:
            noiseData[i][j] = (230, 217, 51)
        elif noiseData[i][j] > 20:
            noiseData[i][j] = (0, 0, 192)
        else:
            noiseData[i][j] = (0, 0, 128)

# generate height map
heightMap = Image.fromarray(np.array(noiseData, np.uint8))
heightMap.save('render/colormap.png')

# create window
app = Ursina(title='Procedural Terrain Generation', borderless=False)

# create terrain entity
terrain = Terrain(heightmap='heightmap.png', skip=8)
terrainEntity = Entity(model=terrain, scale=(100, 20, 100), texture='colormap.png', shader=basic_lighting_shader)

# create skybox and camera
Sky()
EditorCamera()

# run engine
app.run()