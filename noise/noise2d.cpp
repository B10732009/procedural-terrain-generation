#include <cmath>
#include <cstdlib>
#include <vector>

#include "noise2d.hpp"

double Noise2D::lerp(double a, double b, double t)
{
  return a + (b - a) * t;
}

double Noise2D::fade(double t)
{
  return t * t * t * ((6 * t - 15) * t + 10);
}

Noise2D::Noise2D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz) // simplified version constructor
    : Noise2D::Noise2D(_seed, _xsz, _ysz, 20.0, 1, 0.5, 2.0)
{
}

Noise2D::Noise2D(std::size_t _seed,    // seed
                 std::size_t _xsz,     // x-direction size
                 std::size_t _ysz,     // y-direction size
                 std::size_t _scale,   // scale
                 std::size_t _octaves, // octaves level
                 double _lacunarity,   // lacunarity
                 double _persistance)  // persistance
    : mSeed(_seed), mXsz(_xsz), mYsz(_ysz), mScale(_scale), mOctaves(_octaves), mLacunarity(_lacunarity),
      mPersistance(_persistance)
{
  // allocate memory for gradients
  // int gradSize = mX * mY * std::pow(lacunarity, octaves - 1) / scale + 1;
  std::size_t gridNum = (mXsz / mScale + 1) * (mYsz / mScale + 1) * std::pow(mLacunarity, mOctaves - 1);
  std::vector<double> grad(gridNum * 2);

  // int gradSize = ((mX / (int)scale + 1) * (mY / (int)scale + 1)) * std::pow(lacunarity, octaves - 1);
  // double *grad = new double[gradSize * 2];

  // generate gradients randomly
  std::srand(mSeed);
  for (std::size_t i = 0; i < grad.size(); i += 2)
  {
    double r = std::rand();
    grad[i] = std::cos(r);
    grad[i + 1] = std::sin(r);
  }

  // allocate memory for data and initialize
  // mData = new double[mX * mY];
  // std::memset(mData, 0, mX * mY * sizeof(double));
  mData = std::vector<double>(mXsz * mYsz, 0.0);

  // calculate the value of all sample points
  for (std::size_t i = 0; i < mXsz; i++)
  {
    for (std::size_t j = 0; j < mYsz; j++)
    {
      for (std::size_t octi = 0; octi < mOctaves; octi++)
      {
        // sample point coordinates
        // multiply by lacunarity value
        double samplePointX = ((double)i / mScale) * std::pow(mLacunarity, octi);
        double samplePointY = ((double)j / mScale) * std::pow(mLacunarity, octi);

        // grid points of sample point
        int gridX0 = (int)samplePointX;
        int gridX1 = (int)samplePointX + 1;
        int gridY0 = (int)samplePointY;
        int gridY1 = (int)samplePointY + 1;

        // gradients of grid point s
        // x
        double gradX00 = grad[(gridX0 * (mYsz / mScale) + gridY0) * 2];
        double gradX01 = grad[(gridX0 * (mYsz / mScale) + gridY1) * 2];
        double gradX10 = grad[(gridX1 * (mYsz / mScale) + gridY0) * 2];
        double gradX11 = grad[(gridX1 * (mYsz / mScale) + gridY1) * 2];
        // y
        double gradY00 = grad[(gridX0 * (mYsz / mScale) + gridY0) * 2 + 1];
        double gradY01 = grad[(gridX0 * (mYsz / mScale) + gridY1) * 2 + 1];
        double gradY10 = grad[(gridX1 * (mYsz / mScale) + gridY0) * 2 + 1];
        double gradY11 = grad[(gridX1 * (mYsz / mScale) + gridY1) * 2 + 1];

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
        mData[i * mYsz + j] += lerpValue * std::pow(mPersistance, octi);
      }
    }
  }

  // // deallocate the memory for gradients
  // delete[] grad;
}

Noise2D::~Noise2D()
{
  // if (mData)
  //   delete[] mData;
}

double Noise2D::operator()(std::size_t idx1, std::size_t idx2) const
{
  // if (t1 < 0 || t1 >= mX || t2 < 0 || t2 >= mY)
  //   throw std::out_of_range("sample point out of range.");
  // return mData[t1 * mY + t2];

  return mData.at(idx1 * mYsz + idx2);
}

std::size_t Noise2D::seed() const
{
  return mSeed;
}

std::size_t Noise2D::xsz() const
{
  return mXsz;
}

std::size_t Noise2D::ysz() const
{
  return mYsz;
}

std::size_t Noise2D::scale() const
{
  return mScale;
}

std::size_t Noise2D::octaves() const
{
  return mOctaves;
}

double Noise2D::lacunarity() const
{
  return mLacunarity;
}

double Noise2D::persistance() const
{
  return mPersistance;
}

std::vector<double> Noise2D::data() const
{
  return mData;
}