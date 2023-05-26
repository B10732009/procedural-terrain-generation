import noise
import matplotlib.pyplot as plt
import random

# check that the read-only attributes can get correctly
def testAttributes():
    seed = random.randint(0, 999999)
    xsz = 10
    n = noise.Noise1D(seed, xsz)
    assert n.seed == seed
    assert n.xsz == xsz
    assert n.scale == 20
    assert n.octaves == 1
    assert n.lacunarity == 1
    assert n.persistance == 1
    for i, elem in enumerate(n.data):
        assert n[i] == elem

# check that same seed will produce same noise value
def testSeed():
    seed = random.randint(0, 999999)
    xsz = 100
    n1 = noise.Noise1D(seed, xsz)
    n2 = noise.Noise1D(seed, xsz)
    assert n1.data == n2.data

# draw graph
def drawGraph():
    seed = 1277
    size = 800
    a = noise.Noise1D(seed, size, 40, 1, 1, 1)
    b = noise.Noise1D(seed, size, 80, 1, 1, 1)
    c = noise.Noise1D(seed, size, 160, 1, 1, 1)
    d = noise.Noise1D(seed, size, 160, 3, 2.0, 0.5)

    x = [i for i in range(size)]
    y1 = [a[i] for i in range(size)]
    y2 = [b[i] for i in range(size)]
    y3 = [c[i] for i in range(size)]
    y4 = [d[i] for i in range(size)]

    plt.figure(figsize=(15, 6))

    plt.subplot(2, 2, 1)
    plt.title('seed = 1277, size = 800, scale = 40, no octave')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.plot(x, y1, color = 'y')

    plt.subplot(2, 2, 2)
    plt.title('seed = 1277, size = 800, scale = 80, no octave')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.plot(x, y2, color = 'c')

    plt.subplot(2, 2, 3)
    plt.title('seed = 1277, size = 800, scale = 160, no octave')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.plot(x, y3, color = 'm')

    plt.subplot(2, 2, 4)
    plt.title('seed = 1277, size = 800, scale = 160, octave = (3, 2.0, 0.5)')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.plot(x, y4, color = 'b')

    plt.subplots_adjust(left = 0.125, bottom = 0.1, right = 0.9, top = 0.9, wspace = 0.2, hspace = 0.35)
    plt.show()

if __name__ == '__main__':
    drawGraph()


    