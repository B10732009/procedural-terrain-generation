import noise
import matplotlib.pyplot as plt
import random
import numpy as np

# check that the read-only attributes can get correctly
def testAttributes():
    seed = random.randint(0, 999999)
    xsz, ysz = 10, 20
    n = noise.Noise2D(seed, xsz, ysz)
    assert n.seed == seed
    assert n.xsz == xsz
    assert n.ysz == ysz
    assert n.scale == 20
    assert n.octaves == 1
    assert n.lacunarity == 1
    assert n.persistance == 1
    for i, elem in enumerate(n.data):
        assert n[int(i / ysz), int(i % ysz)] == elem

# check that same seed will produce same noise value
def testSeed():
    seed = random.randint(0, 999999)
    xsz, ysz = 10, 20
    n1 = noise.Noise2D(seed, xsz, ysz)
    n2 = noise.Noise2D(seed, xsz, ysz)
    assert n1.data == n2.data

# draw graph
def drawGraph():
    seed = 1277
    size = 128
    a = noise.Noise2D(seed, size, size, 20, 1, 1, 1)
    b = noise.Noise2D(seed, size, size, 40, 1, 1, 1)
    c = noise.Noise2D(seed, size, size, 80, 1, 1, 1)
    d = noise.Noise2D(seed, size, size, 40, 3, 2.0, 0.5)

    y1 = [[int((a[i, j] + 0.5) * 200.0) for j in range(size)] for i in range(size)]
    y2 = [[int((b[i, j] + 0.5) * 200.0) for j in range(size)] for i in range(size)]
    y3 = [[int((c[i, j] + 0.5) * 200.0) for j in range(size)] for i in range(size)]
    y4 = [[int((d[i, j] + 0.5) * 200.0) for j in range(size)] for i in range(size)]

    plt.figure(figsize=(15, 6))

    plt.subplot(2, 2, 1)
    plt.title(f'seed = 1277, size = 128, scale = 20, octave = 1')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y1), cmap = 'gray', vmin = 0, vmax = 255)

    plt.subplot(2, 2, 2)
    plt.title('seed = 1277, size = 128, scale = 40, octave = 1')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y2), cmap = 'gray', vmin = 0, vmax = 255)

    plt.subplot(2, 2, 3)
    plt.title('seed = 1277, size = 128, scale = 80, octave = 1')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y3), cmap = 'gray', vmin = 0, vmax = 255)

    plt.subplot(2, 2, 4)
    plt.title('seed = 1277, size = 128, scale = 40, octave = (3, 2.0, 0.5)')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y4), cmap = 'gray', vmin = 0, vmax = 255)

    plt.subplots_adjust(left = 0.125, bottom = 0.1, right = 0.9, top = 0.9, wspace = 0.2, hspace = 0.35)
    plt.show()

if __name__ == '__main__':
    drawGraph()