#include <cmath>
#include <cstdlib>
#include <vector>

#include "noise1d.hpp"

double Noise1D::lerp(double a, double b, double t)
{
  return a + (b - a) * t;
}

double Noise1D::fade(double t)
{
  return t * t * t * ((6 * t - 15) * t + 10);
}

Noise1D::Noise1D(std::size_t _seed, std::size_t _xsz) : Noise1D::Noise1D(_seed, _xsz, 20.0, 1, 0.5, 2.0)
{
}

Noise1D::Noise1D(std::size_t _seed,    // seed
                 std::size_t _xsz,     // x-direction size
                 std::size_t _scale,   // scale
                 std::size_t _octaves, // octaves level
                 double _lacunarity,   // lacunarity
                 double _persistance)  // persistance
    : mSeed(_seed), mXsz(_xsz), mScale(_scale), mOctaves(_octaves), mLacunarity(_lacunarity), mPersistance(_persistance)
{
  // allocate memory for gradients
  std::size_t gridNum = (mXsz / mScale + 1) * std::pow(mLacunarity, mOctaves - 1);
  std::vector<double> grad(gridNum);

  // generate gradients randomly
  std::srand(mSeed);
  for (std::size_t i = 0; i < grad.size(); i++)
    grad[i] = std::cos(std::rand());

  // allocate memory for data and initialize
  mData = std::vector<double>(mXsz, 0.0);

  // calculate the value of all sample points
  for (std::size_t i = 0; i < mXsz; i++)
  {
    for (std::size_t octi = 0; octi < mOctaves; octi++)
    {
      // sample point coordinates
      // multiply by lacunarity value
      double samplePointX = ((double)i / mScale) * std::pow(mLacunarity, octi);

      // grid points of sample point
      int gridX0 = (int)samplePointX;
      int gridX1 = (int)samplePointX + 1;

      // gradients of grid point s
      double grad0 = grad[gridX0];
      double grad1 = grad[gridX1];

      // interpolation point
      double lerpPoint = samplePointX - gridX0;

      // influence values : dot(gradient * grid_point_to_sample_point_vector)
      double influence0 = grad0 * lerpPoint;
      double influence1 = grad1 * (lerpPoint - 1);

      // interpolate the influence values with fading function
      double lerpValue = lerp(influence0, influence1, fade(lerpPoint));

      // interpolate the influence values with fading function
      // multiply by persistence value
      mData[i] += lerpValue * std::pow(mPersistance, octi);
    }
  }
}

Noise1D::~Noise1D()
{
}

double Noise1D::operator()(std::size_t idx) const
{
  return mData.at(idx);
}

std::size_t Noise1D::seed() const
{
  return mSeed;
}

std::size_t Noise1D::xsz() const
{
  return mXsz;
}

std::size_t Noise1D::scale() const
{
  return mScale;
}

std::size_t Noise1D::octaves() const
{
  return mOctaves;
}

double Noise1D::lacunarity() const
{
  return mLacunarity;
}

double Noise1D::persistance() const
{
  return mPersistance;
}

std::vector<double> Noise1D::data() const
{
  return mData;
}