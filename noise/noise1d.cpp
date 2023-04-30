#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "noise1d.hpp"

double Noise1D::lerp(double a, double b, double t)
{
  return a + (b - a) * t;
}

double Noise1D::fade(double t)
{
  return t * t * t * ((6 * t - 15) * t + 10);
}

Noise1D::Noise1D(std::size_t seed, std::size_t x) : Noise1D::Noise1D(seed, x, 20.0, 1, 0.5, 2.0)
{
}

Noise1D::Noise1D(std::size_t seed, std::size_t x, double scale, int octaves, double lacunarity, double persistance)
    : mSeed(seed), mX(x)
{
  // allocate memory for gradients
  int gradSize = mX * std::pow(lacunarity, octaves - 1) / scale + 1;
  double *grad = new double[gradSize];

  // generate gradients randomly
  std::srand(mSeed);
  for (int i = 0; i < gradSize; i++)
    grad[i] = std::cos(std::rand());

  // allocate memory for data and initialize
  mData = new double[mX];
  std::memset(mData, 0, mX * sizeof(double));

  // calculate the value of all sample points
  for (int i = 0; i < (int)mX; i++)
  {
    for (int j = 0; j < octaves; j++)
    {
      // sample point coordinates
      // multiply by lacunarity value
      double samplePoint = ((double)i / scale) * std::pow(lacunarity, j);

      // grid points of sample point
      int grid0 = (int)samplePoint;
      int grid1 = (int)samplePoint + 1;

      // gradients of grid point s
      double grad0 = grad[grid0];
      double grad1 = grad[grid1];

      // interpolation point
      double lerpPoint = samplePoint - grid0;

      // influence values : dot(gradient * grid_point_to_sample_point_vector)
      double influence0 = grad0 * lerpPoint;
      double influence1 = grad1 * (lerpPoint - 1);

      // interpolate the influence values with fading function
      // multiply by persistence value
      mData[i] += lerp(influence0, influence1, fade(lerpPoint)) * std::pow(persistance, j);
    }
  }

  // deallocate the memory for gradients
  delete[] grad;
}

Noise1D::~Noise1D()
{
  if (mData)
    delete[] mData;
}

double Noise1D::operator()(std::size_t t) const
{
  if (t < 0 || t >= mX)
    throw std::out_of_range("sample point out of range.");
  return mData[t];
}

std::size_t Noise1D::seed() const
{
  return mSeed;
}

std::size_t Noise1D::x() const
{
  return mX;
}

const double *Noise1D::data() const
{
  return (const double *)mData;
}