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
    a = noise.Noise3D(seed, size, size, size, 5, 1, 1, 1)
    # b = noise.Noise2D(seed, size, size, 80, 1, 1, 1)
    # c = noise.Noise2D(seed, size, size, 160, 1, 1, 1)
    # d = noise.Noise2D(seed, size, size, 160, 3, 2.0, 0.5)
    x=[i for i in range(size)]
    y1=[[((a[i, j, 0]+0.5)*200.0) for j in range(size)] for i in range(size)]
    # y1=[[a[i, j, 0] for j in range(size)] for i in range(size)]
    # # print(y1)
    # y2=[[int((b[i, j]+0.5)*255.0) for j in range(size)] for i in range(size)]
    # y3=[[int((c[i, j]+0.5)*255.0) for j in range(size)] for i in range(size)]
    # y4=[[int((d[i, j]+0.5)*255.0) for j in range(size)] for i in range(size)]

    for a in y1:
        print(a)

    plt.figure(figsize=(15, 6))

    plt.subplot(2, 2, 1)
    plt.title('seed = 1277, size = 800, scale = 40, no octave')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.imshow(np.array(y1), cmap='gray', vmin=0, vmax=255)

    # plt.subplot(2, 2, 2)
    # plt.title('seed = 1277, size = 800, scale = 40, no octave')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.plot(np.array(x), np.array(y1[0]), color='y')
    
    # plt.subplot(2, 2, 2)
    # plt.title('seed = 1277, size = 800, scale = 80, no octave')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.imshow(y2, cmap='gray', vmin=0, vmax=255)

    # plt.subplot
    # (2, 2, 3)
    # plt.title('seed = 1277, size = 800, scale = 160, no octave')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.imshow(y3, cmap='gray', vmin=0, vmax=255)

    # plt.subplot(2, 2, 4)
    # plt.title('seed = 1277, size = 800, scale = 160, octave = (3, 2.0, 0.5)')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.imshow(y4, cmap='gray', vmin=0, vmax=255)


    # plt.subplot(2, 2, 3)
    # plt.title('seed = 1277, size = 800, scale = 160, no octave')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.plot(x, y3, color='m')

    # plt.subplot(2, 2, 4)
    # plt.title('seed = 1277, size = 800, scale = 160, octave = (3, 2.0, 0.5)')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.plot(x, y4, color='b')

    plt.subplots_adjust(left=0.125, bottom=0.1, right=0.9, top=0.9, wspace=0.2, hspace=0.35)
    plt.show()

def aaa():
    np.random.seed(19680801)
    data = np.random.random((20, 20, 20))



    seed = 1277
    size = 100
    a = noise.Noise3D(seed, size, size, size, 20, 3, 2.0, 0.5)
    # b = noise.Noise2D(seed, size, size, 80, 1, 1, 1)
    # c = noise.Noise2D(seed, size, size, 160, 1, 1, 1)
    # d = noise.Noise2D(seed, size, size, 160, 3, 2.0, 0.5)
    x=[i for i in range(size)]
    y1=[[[(a[i, j, k]+0.5)*200.0 for k in range(size)] for j in range(size)] for i in range(size)]



    fig, ax = plt.subplots()

    fidx = 0
    while True:
        ax.clear()
        ax.imshow(y1[fidx], vmin=0, vmax=255)
        fidx = (fidx + 1) % size
        ax.set_title(f"frame {fidx}")
        # Note that using time.sleep does *not* work here!
        plt.pause(0.000001)

if __name__ == '__main__':
    # drawGraph()
    aaa()