import noise
import matplotlib.pyplot as plt
import random
import numpy as np

# check that the read-only attributes can get correctly
def testAttributes():
    seed = random.randint(0, 999999)
    size = 800
    n = noise.Noise2D(seed, size, size)
    assert n.seed == seed
    assert n.x == size
    assert n.y == size

# check that same seed will produce same noise value
def testSeed():
    seed = random.randint(0, 999999)
    size = 800
    n1 = noise.Noise2D(seed, size, size)
    n2 = noise.Noise2D(seed, size, size)
    assert n1.data == n2.data

# check that octave calculation is correct
# def testOctave():
#     seed = random.randint(0, 999999)
#     size = 800
#     a = noise.Noise2D(seed, size, size, 40, 1, 1, 1)
#     b = noise.Noise2D(seed, size, size, 80, 1, 1, 1)
#     c = noise.Noise2D(seed, size, size, 160, 1, 1, 1)
#     d = noise.Noise2D(seed, size, size, 160, 3, 2.0, 0.5)
#     for i in range(size):
#         for j in range(size):
#             assert (a[i, j] / 4.0 + b[i, j] / 2.0 + c[i, j]) - d[i, j] < 1e-6

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
    plt.imshow(np.array(y1), cmap='gray', vmin=0, vmax=255)

    plt.subplot(2, 2, 2)
    plt.title('seed = 1277, size = 128, scale = 40, octave = 1')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y2), cmap='gray', vmin=0, vmax=255)

    plt.subplot(2, 2, 3)
    plt.title('seed = 1277, size = 128, scale = 80, octave = 1')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y3), cmap='gray', vmin=0, vmax=255)

    plt.subplot(2, 2, 4)
    plt.title('seed = 1277, size = 128, scale = 40, octave = (3, 2.0, 0.5)')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y4), cmap='gray', vmin=0, vmax=255)

    plt.subplots_adjust(left=0.125, bottom=0.1, right=0.9, top=0.9, wspace=0.2, hspace=0.35)
    plt.show()

if __name__ == '__main__':
    drawGraph()