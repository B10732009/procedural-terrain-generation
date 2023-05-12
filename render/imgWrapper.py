from PIL import Image as pImage
import numpy as np

class imgWrapper:
    def __init__(self, initData):
        if isinstance(initData, str):
            img = pImage.open(initData)
            self.height = img.size[0]
            self.width = img.size[1]
            self.pixels = list(img.getdata())
            self.pixels = [self.pixels[i * self.width : (i + 1) * self.width] for i in range(self.height)]
        else:
            self.pixels = initData
            self.height = len(self.pixels)
            self.width = len(self.pixels[0])

    def getHeight(self):
        return self.height
    
    def getWidth(self):
        return self.width
    
    def getPixels(self):
        return self.pixels
    
    def getPixel(self, i, j):
        return self.pixels[i % self.height][j % self.width][0:3]
    
    def save(self, filename):
        img = pImage.fromarray(np.array(self.pixels, np.uint8))
        img.save(filename)
