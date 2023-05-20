from ursina import *
from ursina.shaders import basic_lighting_shader
from noise import Noise2D
from imgWrapper import imgWrapper

def readConf(filename):
    conf = {}
    f = open(filename, 'r')
    if f.readable:
        for line in f.readlines():
            item = line[:-1].split()
            conf[item[0]] = item[1]
    f.close()
    return conf
                
def heightCurve(x, coef):
    if x < 0:
        return 0
    elif x > 1:
        return 1
    else:
        return x ** coef

def falloff(x, coef1, coef2):
    if x < 0:
        return 0
    elif x > 1:
        return 1
    else:
        return (x ** coef1) / ((x ** coef1) + (coef2 - coef2 * x) ** coef1)

#################### main script ####################

# read config file
conf = readConf('render/render.conf')
conf = {
    'seed': int(conf['seed']),
    'size': int(conf['size']),
    'scale': int(conf['scale']),
    'octave': int(conf['octave']),
    'lacunarity': float(conf['lacunarity']),
    'persistence': float(conf['persistence']),
    'usingTextures': int(conf['usingTextures']),
    'usingFalloffMap': int(conf['usingFalloffMap']),
    'heightCurveCoef': float(conf['heightCurveCoef']),
    'falloffCoef1': float(conf['falloffCoef1']),
    'falloffCoef2': float(conf['falloffCoef2'])
}

# generate noise
noiseObj = Noise2D(conf['seed'], conf['size'], conf['size'], conf['scale'], conf['octave'], conf['lacunarity'], conf['persistence'])
noiseData = [[noiseObj[i, j] for j in range(conf['size'])] for i in range(conf['size'])]

# adjust details of noise values
for i in range(conf['size']):
    for j in range(conf['size']):
        # add height curves
        noiseData[i][j] = heightCurve(noiseData[i][j] + 0.5, conf['heightCurveCoef'])
        # add falloff map
        if conf['usingFalloffMap']:
            noiseData[i][j] *= falloff(min(i, j, conf['size'] - i, conf['size'] - j) / conf['size'], conf['falloffCoef1'], conf['falloffCoef2'])
        # scale the value
        noiseData[i][j] = int(noiseData[i][j] * 180.0)
        # check value validation
        if noiseData[i][j] < 0:
            noiseData[i][j] = 0
        elif noiseData[i][j] > 255:
            noiseData[i][j] = 255

# generate height map
heightMap = imgWrapper(noiseData)
heightMap.save('render/heightmap.png')

# generate level lookup table
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
if conf['usingTextures']:
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
    for i in range(conf['size']):
        for j in range(conf['size']):
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
    for i in range(conf['size']):
        for j in range(conf['size']):
            noiseData[i][j] = tLUT[lvLUT[noiseData[i][j]]]

# generate color map
colorMap = imgWrapper(noiseData)
colorMap.save('render/colormap.png')

# create window
app = Ursina(title='Procedural Terrain Generation', borderless=False)

# create terrain entity
terrain = Terrain(heightmap='heightmap.png', skip=8)
terrainEntity = Entity(model=terrain, scale=(conf['size'] / 5.12, 15, conf['size'] / 5.12), texture='colormap.png', shader=basic_lighting_shader)

# create skybox and camera
Sky()
EditorCamera()

# run engine
app.run()