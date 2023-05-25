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
    size = 40
    leng = 128
    n = noise.Noise3D(seed, leng, size, size, 10, 3, 2.0, 0.5)
    nData = [[[(n[i, j, k] + 0.5) * 200.0 for k in range(size)] for j in range(size)] for i in range(leng)]
    
    fig, ax = plt.subplots()
    for i, frame in enumerate(nData):
        ax.clear()
        ax.imshow(np.array(frame), cmap='gray', vmin=0, vmax=255)
        ax.set_title(f"frame {i}")
        plt.pause(0.01) # Note that using time.sleep does *not* work here!

if __name__ == '__main__':
    drawGraph()