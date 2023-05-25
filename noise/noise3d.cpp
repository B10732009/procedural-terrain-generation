#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <array>
#include <vector>

#include "noise3d.hpp"

double Noise3D::lerp(double a, double b, double t)
{
  return a + (b - a) * t;
}

double Noise3D::fade(double t)
{
  return t * t * t * ((6 * t - 15) * t + 10);
}

int Noise3D::loc(int x, int y, int z)
{
  return x * mY + y + z * (mX * mY);
}

int Noise3D::sloc(int x, int y, int z, int scale)
{
  return x * (mY / scale) + y + z * ((mX / scale) * (mY / scale));
}

Noise3D::Noise3D(std::size_t seed, std::size_t x, std::size_t y, std::size_t z)
    : Noise3D::Noise3D(seed, x, y, z, 20.0, 1, 0.5, 2.0)
{
}

Noise3D::Noise3D(std::size_t seed, std::size_t x, std::size_t y, std::size_t z, double scale, int octaves,
                 double lacunarity, double persistance)
    : mSeed(seed), mX(x), mY(y), mZ(z)
{
  // allocate memory for gradients
  int gradSize = (mX / scale + 1) * (mY / scale + 1) * (mZ / scale + 1) * std::pow(lacunarity, octaves - 1) * 2;
  double *grad = new double[gradSize * 3];

  // generate gradients randomly
  std::srand(mSeed);
  for (int i = 0; i < gradSize * 3; i += 3)
  {
    int r1 = std::rand(); // std::rand();
    int r2 = std::rand();
    grad[i] = std::sin(r1) * std::cos(r2);
    grad[i + 1] = std::sin(r1) * std::sin(r2);
    grad[i + 2] = std::cos(r1);
    // std::cout << "PPPPPPPPPPP " << grad[i] * grad[i] + grad[i + 1] * grad[i + 1] + grad[i + 2] * grad[i + 2]
    //           << std::endl;
  }

  // std::cout << "==============\n";
  // for (int i = 0; i < gradSize * 3; i++)
  //   std::cout << grad[i] << std::endl;
  // std::cout << "==============\n";
  // allocate memory for data and initialize
  mData = new double[mX * mY * mZ];
  std::memset(mData, 0, mX * mY * mZ * sizeof(double));

  // for (int i = 0; i < mX * mY * mZ; i++)
  //   mData[i] = 1.234;

  // calculate the value of all sample points
  for (int i = 0; i < (int)mX; i++)
  {
    for (int j = 0; j < (int)mY; j++)
    {
      for (int m = 0; m < (int)mZ; m++)
      {
        for (int k = 0; k < octaves; k++)
        {
          // sample point coordinates
          // multiply by lacunarity value
          double samplePointX = ((double)i / scale) * std::pow(lacunarity, k);
          double samplePointY = ((double)j / scale) * std::pow(lacunarity, k);
          double samplePointZ = ((double)m / scale) * std::pow(lacunarity, k);

          // grid points of sample point
          int gridX0 = (int)samplePointX;
          int gridX1 = (int)samplePointX + 1;
          int gridY0 = (int)samplePointY;
          int gridY1 = (int)samplePointY + 1;
          int gridZ0 = (int)samplePointZ;
          int gridZ1 = (int)samplePointZ + 1;

          // std::cout << gridX0 << " " << gridX1 << std::endl;
          // std::cout << gridY0 << " " << gridY1 << std::endl;
          // std::cout << gridZ0 << " " << gridZ1 << std::endl;
          // std::cout << std::endl;

          // gradients of grid point s
          // x
          double gradX000 = grad[sloc(gridX0, gridY0, gridZ0, scale) * 3];
          double gradX001 = grad[sloc(gridX0, gridY0, gridZ1, scale) * 3];
          double gradX010 = grad[sloc(gridX0, gridY1, gridZ0, scale) * 3];
          double gradX011 = grad[sloc(gridX0, gridY1, gridZ1, scale) * 3];
          double gradX100 = grad[sloc(gridX1, gridY0, gridZ0, scale) * 3];
          double gradX101 = grad[sloc(gridX1, gridY0, gridZ1, scale) * 3];
          double gradX110 = grad[sloc(gridX1, gridY1, gridZ0, scale) * 3];
          double gradX111 = grad[sloc(gridX1, gridY1, gridZ1, scale) * 3];
          // y
          double gradY000 = grad[sloc(gridX0, gridY0, gridZ0, scale) * 3 + 1];
          double gradY001 = grad[sloc(gridX0, gridY0, gridZ1, scale) * 3 + 1];
          double gradY010 = grad[sloc(gridX0, gridY1, gridZ0, scale) * 3 + 1];
          double gradY011 = grad[sloc(gridX0, gridY1, gridZ1, scale) * 3 + 1];
          double gradY100 = grad[sloc(gridX1, gridY0, gridZ0, scale) * 3 + 1];
          double gradY101 = grad[sloc(gridX1, gridY0, gridZ1, scale) * 3 + 1];
          double gradY110 = grad[sloc(gridX1, gridY1, gridZ0, scale) * 3 + 1];
          double gradY111 = grad[sloc(gridX1, gridY1, gridZ1, scale) * 3 + 1];
          // z
          double gradZ000 = grad[sloc(gridX0, gridY0, gridZ0, scale) * 3 + 2];
          double gradZ001 = grad[sloc(gridX0, gridY0, gridZ1, scale) * 3 + 2];
          double gradZ010 = grad[sloc(gridX0, gridY1, gridZ0, scale) * 3 + 2];
          double gradZ011 = grad[sloc(gridX0, gridY1, gridZ1, scale) * 3 + 2];
          double gradZ100 = grad[sloc(gridX1, gridY0, gridZ0, scale) * 3 + 2];
          double gradZ101 = grad[sloc(gridX1, gridY0, gridZ1, scale) * 3 + 2];
          double gradZ110 = grad[sloc(gridX1, gridY1, gridZ0, scale) * 3 + 2];
          double gradZ111 = grad[sloc(gridX1, gridY1, gridZ1, scale) * 3 + 2];

          // std::cout << gradX000 << " " << gradX001 << " " << gradX010 << " " << gradX011 << std::endl;
          // std::cout << loc(gridX0, gridY0, gridZ1) * 3 << std::endl;
          // interpolation point
          double lerpPointX = samplePointX - gridX0;
          double lerpPointY = samplePointY - gridY0;
          double lerpPointZ = samplePointZ - gridZ0;

          // influence values : dot(gradient * grid_point_to_sample_point_vector)
          double influence000 = gradX000 * lerpPointX + gradY000 * lerpPointY + gradZ000 * lerpPointZ;
          double influence001 = gradX001 * lerpPointX + gradY001 * lerpPointY + gradZ001 * (lerpPointZ - 1);
          double influence010 = gradX010 * lerpPointX + gradY010 * (lerpPointY - 1) + gradZ010 * lerpPointZ;
          double influence011 = gradX011 * lerpPointX + gradY011 * (lerpPointY - 1) + gradZ011 * (lerpPointZ - 1);
          double influence100 = gradX100 * (lerpPointX - 1) + gradY100 * lerpPointY + gradZ100 * lerpPointZ;
          double influence101 = gradX101 * (lerpPointX - 1) + gradY101 * lerpPointY + gradZ101 * (lerpPointZ - 1);
          double influence110 = gradX110 * (lerpPointX - 1) + gradY110 * (lerpPointY - 1) + gradZ110 * lerpPointZ;
          double influence111 = gradX111 * (lerpPointX - 1) + gradY111 * (lerpPointY - 1) + gradZ111 * (lerpPointZ - 1);

          // std::cout << "inf " << influence000 << " " << influence001 << " " << influence010 << " " << influence011
          //           << std::endl;
          // double influence01 = gradX01 * lerpPointX + gradY01 * (lerpPointY - 1);
          // double influence10 = gradX10 * (lerpPointX - 1) + gradY10 * lerpPointY;
          // double influence11 = gradX11 * (lerpPointX - 1) + gradY11 * (lerpPointY - 1);

          // interpolate the influence values with fading function
          // double lerpValue = lerp(lerp(influence00, influence01, fade(lerpPointY)),
          //                         lerp(influence10, influence11, fade(lerpPointY)), fade(lerpPointX));

          double lerpValue = lerp(lerp(lerp(influence000, influence001, fade(lerpPointZ)),
                                       lerp(influence010, influence011, fade(lerpPointZ)), fade(lerpPointY)),
                                  lerp(lerp(influence100, influence101, fade(lerpPointZ)),
                                       lerp(influence110, influence111, fade(lerpPointZ)), fade(lerpPointY)),
                                  fade(lerpPointX));

          // multiply by persistence value
          // std::cout << "uuuuuu" << lerpValue * std::pow(persistance, k) << std::endl;
          mData[loc(i, j, m)] += lerpValue * std::pow(persistance, k);
        }
      }
    }
  }

  // deallocate the memory for gradients
  delete[] grad;
}

Noise3D::~Noise3D()
{
  if (mData)
    delete[] mData;
}

double Noise3D::operator()(std::size_t t1, std::size_t t2, std::size_t t3) const
{
  if (t1 < 0 || t1 >= mX || t2 < 0 || t2 >= mY || t3 < 0 || t3 >= mZ)
    throw std::out_of_range("sample point out of range.");
  // std::cout << mData[t1 * mY + t2 + t3 * (mX * mY)] << std::endl;
  return mData[t1 * mY + t2 + t3 * (mX * mY)];
}

std::size_t Noise3D::seed() const
{
  return mSeed;
}

std::size_t Noise3D::x() const
{
  return mX;
}

std::size_t Noise3D::y() const
{
  return mY;
}

std::size_t Noise3D::z() const
{
  return mZ;
}

const double *Noise3D::data() const
{
  return (const double *)mData;
}