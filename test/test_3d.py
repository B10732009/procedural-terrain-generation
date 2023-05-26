import noise
import matplotlib.pyplot as plt
import matplotlib.animation as pltanim
import random
import numpy as np

# check that the read-only attributes can get correctly
def testAttributes():
    seed = random.randint(0, 999999)
    xsz, ysz, zsz = 10, 20, 30
    n = noise.Noise3D(seed, xsz, ysz, zsz)
    assert n.seed == seed
    assert n.xsz == xsz
    assert n.ysz == ysz
    assert n.zsz == zsz
    assert n.scale == 20
    assert n.octaves == 1
    assert n.lacunarity == 1
    assert n.persistance == 1
    for i, elem in enumerate(n.data):
        assert n[int(int(i % (xsz * ysz)) / ysz), int(i % ysz), int(i / (xsz * ysz))] == elem

# check that same seed will produce same noise value
def testSeed():
    seed = random.randint(0, 999999)
    xsz, ysz, zsz = 10, 20, 30
    n1 = noise.Noise3D(seed, xsz, ysz, zsz)
    n2 = noise.Noise3D(seed, xsz, ysz, zsz)
    assert n1.data == n2.data

# draw graph
def drawGraph():
    seed = 1277
    size = 40
    leng = 128
    n = noise.Noise3D(seed, leng, size, size, 10, 3, 2.0, 0.5)
    nData = [[[(n[i, j, k] + 0.5) * 200.0 for k in range(size)] for j in range(size)] for i in range(leng)]
    
    # display animation
    fig, ax = plt.subplots()

    def update(idx):
        ax.clear()
        ax.imshow(np.array(nData[idx]), cmap='gray', vmin=0, vmax=255)
        ax.set_title(f"seed = 1277, size = 40, scale = 10, octave = (3, 2.0, 0.5), frame {idx}")

    anim = pltanim.FuncAnimation(fig, update, frames = 128, interval = 100)
    anim.save('test/noise3d.gif')
    plt.show()

if __name__ == '__main__':
    drawGraph()