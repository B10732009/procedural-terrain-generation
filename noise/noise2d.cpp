#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "noise2d.hpp"

double Noise2D::lerp(double a, double b, double t)
{
  return a + (b - a) * t;
}

double Noise2D::fade(double t)
{
  return t * t * t * ((6 * t - 15) * t + 10);
}

Noise2D::Noise2D(std::size_t seed, std::size_t x, std::size_t y) : Noise2D::Noise2D(seed, x, y, 20.0, 1, 0.5, 2.0)
{
}

Noise2D::Noise2D(std::size_t seed, std::size_t x, std::size_t y, double scale, int octaves, double lacunarity,
                 double persistance)
    : mSeed(seed), mX(x), mY(y)
{
  // allocate memory for gradients
  int gradSize = mX * mY * std::pow(lacunarity, octaves - 1) / scale + 1;
  double *grad = new double[gradSize * 2];

  // generate gradients randomly
  std::srand(mSeed);
  for (int i = 0; i < gradSize * 2; i += 2)
  {
    double r = std::rand();
    grad[i] = std::cos(r);
    grad[i + 1] = std::sin(r);
  }

  // allocate memory for data and initialize
  mData = new double[mX * mY];
  std::memset(mData, 0, mX * mY * sizeof(double));

  // calculate the value of all sample points
  for (int i = 0; i < (int)mX; i++)
  {
    for (int j = 0; j < (int)mY; j++)
    {
      for (int k = 0; k < octaves; k++)
      {
        // sample point coordinates
        // multiply by lacunarity value
        double samplePointX = ((double)i / scale) * std::pow(lacunarity, k);
        double samplePointY = ((double)j / scale) * std::pow(lacunarity, k);

        // grid points of sample point
        int gridX0 = (int)samplePointX;
        int gridX1 = (int)samplePointX + 1;
        int gridY0 = (int)samplePointY;
        int gridY1 = (int)samplePointY + 1;

        // gradients of grid point s
        // x
        double gradX00 = grad[(gridX0 * mY + gridY0) * 2];
        double gradX01 = grad[(gridX0 * mY + gridY1) * 2];
        double gradX10 = grad[(gridX1 * mY + gridY0) * 2];
        double gradX11 = grad[(gridX1 * mY + gridY1) * 2];
        // y
        double gradY00 = grad[(gridX0 * mY + gridY0) * 2 + 1];
        double gradY01 = grad[(gridX0 * mY + gridY1) * 2 + 1];
        double gradY10 = grad[(gridX1 * mY + gridY0) * 2 + 1];
        double gradY11 = grad[(gridX1 * mY + gridY1) * 2 + 1];

        // interpolation point
        double lerpPointX = samplePointX - gridX0;
        double lerpPointY = samplePointY - gridY0;

        // influence values : dot(gradient * grid_point_to_sample_point_vector)
        double influence00 = gradX00 * lerpPointX + gradY00 * lerpPointY;
        double influence01 = gradX01 * lerpPointX + gradY01 * (lerpPointY - 1);
        double influence10 = gradX10 * (lerpPointX - 1) + gradY10 * lerpPointY;
        double influence11 = gradX11 * (lerpPointX - 1) + gradY11 * (lerpPointY - 1);

        // interpolate the influence values with fading function
        double lerpValue = lerp(lerp(influence00, influence01, fade(lerpPointY)),
                                lerp(influence10, influence11, fade(lerpPointY)), fade(lerpPointX));

        // multiply by persistence value
        mData[i * mY + j] += lerpValue * std::pow(persistance, k);
      }
    }
  }

  // deallocate the memory for gradients
  delete[] grad;
}

Noise2D::~Noise2D()
{
  if (mData)
    delete[] mData;
}

double Noise2D::operator()(std::size_t t1, std::size_t t2) const
{
  if (t1 < 0 || t1 >= mX || t2 < 0 || t2 >= mY)
    throw std::out_of_range("sample point out of range.");
  return mData[t1 * mY + t2];
}

std::size_t Noise2D::seed() const
{
  return mSeed;
}

std::size_t Noise2D::x() const
{
  return mX;
}

std::size_t Noise2D::y() const
{
  return mY;
}

const double *Noise2D::data() const
{
  return (const double *)mData;
}