#include <cmath>
#include <cstdlib>
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

Noise1D::Noise1D(std::size_t seed, std::size_t x) : Noise1D::Noise1D(seed, x, 10.0, 1, 0.5, 2.0)
{
}

Noise1D::Noise1D(std::size_t seed, std::size_t x, double scale, int octaves, double lacunarity, double persistance)
    : mSeed(seed), mX(x)
{
  // generate gradients randomly
  std::size_t gradSize = mX / scale + 1;
  double *grad = new double[gradSize];

  std::srand(mSeed);
  for (std::size_t i = 0; i < gradSize; i++)
    grad[i] = std::cos(std::rand());

  // calculate the value of all sample points
  mData = new double[mX];
  for (std::size_t i = 0; i < mX; i++)
  {
    // sample point coordinates
    double samplePoint = (double)i / scale;
    int sample0 = (int)samplePoint;
    int sample1 = (int)samplePoint + 1;

    // sample point gradients
    double grad0 = grad[sample0];
    double grad1 = grad[sample1];

    // interpolation point
    double lerpPoint = samplePoint - sample0;

    // influence values : dot(gradient * grid_point_to_sample_point_vector)
    double influence0 = grad0 * lerpPoint;
    double influence1 = grad1 * (lerpPoint - 1);

    mData[i] = lerp(influence0, influence1, fade(lerpPoint));
  }

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