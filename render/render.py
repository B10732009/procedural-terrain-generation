from ursina import *
from ursina.shaders import basic_lighting_shader
from PIL import Image
from noise import Noise2D
from imgWrapper import imgWrapper
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
    b = 0.1
    if x < 0:
        return 0
    elif x > 1:
        return 1
    else:
        return (x ** a) / ((x ** a) + (b - b * x) ** a)

# noise parameters
seed = 1277
size = 512
scale = 160
octave = 5
lacunarity = 2.0
persistence = 0.5

# render parameters
usingTextures = 1

# generate noise
noiseObj = Noise2D(seed, size, size, scale, octave, lacunarity, persistence)
noiseData = [[int(heightCurve(noiseObj[i, j] + 0.5) * falloff(min(i, j, size - i, size - j) / size) * 180.0) for j in range(size)] for i in range(size)]

# deal with out of range (0~255)
for i in range(size):
    for j in range(size):
        if noiseData[i][j] < 0:
            noiseData[i][j] = 0
        elif noiseData[i][j] > 255:
            noiseData[i][j] = 255

# generate height map
heightMap = imgWrapper(noiseData)
heightMap.save('render/heightmap.png')

lvLUT = \
    [0 for i in range(16)] + \
    [1 for i in range(15)] + \
    [2 for i in range(20)] + \
    [3 for i in range(40)] + \
    [4 for i in range(30)] + \
    [5 for i in range(20)] + \
    [6 for i in range(40)] + \
    [7 for i in range(74)]

# transform height map data to color map
# using textures
if usingTextures:
    tLUT = [
        imgWrapper('render/assets/water2.png'),
        imgWrapper('render/assets/water1.png'),
        imgWrapper('render/assets/sand.png'),
        imgWrapper('render/assets/grass.png'),
        imgWrapper('render/assets/rock3.png'),
        imgWrapper('render/assets/rock2.png'),
        imgWrapper('render/assets/rock1.png'),
        imgWrapper('render/assets/snow.png')
    ]
    for i in range(size):
        for j in range(size):
            noiseData[i][j] = tLUT[lvLUT[noiseData[i][j]]].getPixel(i, j)
# using colors
else:
    tLUT = [
        (0, 0, 128),
        (0, 0, 192),
        (230, 217, 51),
        (0, 128, 0),
        (210, 105, 30),
        (160, 82, 45),
        (77, 31, 0),
        (250, 240, 230)
    ]
    for i in range(size):
        for j in range(size):
            noiseData[i][j] = tLUT[lvLUT[noiseData[i][j]]]

# generate color map
colorMap = imgWrapper(noiseData)
colorMap.save('render/colormap.png')

# create window
app = Ursina(title='Procedural Terrain Generation', borderless=False)

# create terrain entity
terrain = Terrain(heightmap='heightmap.png', skip=8)
terrainEntity = Entity(model=terrain, scale=(100, 15, 100), texture='colormap.png', shader=basic_lighting_shader)

# create skybox and camera
Sky()
EditorCamera()

# run engine
app.run()