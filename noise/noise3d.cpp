#include <cmath>
#include <cstdlib>
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

Noise3D::Noise3D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz, std::size_t _zsz) //
    : Noise3D::Noise3D(_seed, _xsz, _ysz, _zsz, 20, 1, 1, 1)
{
}

Noise3D::Noise3D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz, std::size_t _zsz, std::size_t _scale, std::size_t _octaves, double _lacunarity, double _persistance) //
    : mSeed(_seed), mXsz(_xsz), mYsz(_ysz), mZsz(_zsz), mScale(_scale), mOctaves(_octaves), mLacunarity(_lacunarity), mPersistance(_persistance)
{
  // allocate memory for gradients
  std::size_t gridNum = (mXsz / mScale + 1) * (mYsz / mScale + 1) * (mZsz / mScale + 1) * std::pow(mLacunarity, mOctaves - 1);
  std::vector<double> grad(gridNum * 3);

  // generate gradients randomly
  std::srand(mSeed);
  for (std::size_t i = 0; i < grad.size(); i += 3)
  {
    int r1 = std::rand();
    int r2 = std::rand();
    grad[i] = std::sin(r1) * std::cos(r2);
    grad[i + 1] = std::sin(r1) * std::sin(r2);
    grad[i + 2] = std::cos(r1);
  }

  // allocate memory for data and initialize
  mData = std::vector<double>(mXsz * mYsz * mZsz, 0.0);

  // calculate the value of all sample points
  for (std::size_t i = 0; i < mXsz; i++)
  {
    for (std::size_t j = 0; j < mYsz; j++)
    {
      for (std::size_t k = 0; k < mZsz; k++)
      {
        for (std::size_t octi = 0; octi < mOctaves; octi++)
        {
          // sample point coordinates
          // multiply by lacunarity value
          double samplePointX = ((double)i / mScale) * std::pow(mLacunarity, octi);
          double samplePointY = ((double)j / mScale) * std::pow(mLacunarity, octi);
          double samplePointZ = ((double)k / mScale) * std::pow(mLacunarity, octi);

          // grid points of sample point
          int gridX0 = (int)samplePointX;
          int gridX1 = (int)samplePointX + 1;
          int gridY0 = (int)samplePointY;
          int gridY1 = (int)samplePointY + 1;
          int gridZ0 = (int)samplePointZ;
          int gridZ1 = (int)samplePointZ + 1;

          // gradients of grid point s
          // x
          double gradX000 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX001 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX010 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX011 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX100 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX101 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX110 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          double gradX111 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3];
          // y
          double gradY000 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY001 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY010 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY011 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY100 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY101 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY110 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          double gradY111 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 1];
          // z
          double gradZ000 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ001 = grad[(gridX0 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ010 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ011 = grad[(gridX0 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ100 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ101 = grad[(gridX1 * (mYsz / mScale) + gridY0 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ110 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ0 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];
          double gradZ111 = grad[(gridX1 * (mYsz / mScale) + gridY1 + gridZ1 * (mXsz / mScale) * (mYsz / mScale)) * 3 + 2];

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

          // interpolate the influence values with fading function
          double lerpValue = lerp(lerp(lerp(influence000, influence001, fade(lerpPointZ)), lerp(influence010, influence011, fade(lerpPointZ)), fade(lerpPointY)),
                                  lerp(lerp(influence100, influence101, fade(lerpPointZ)), lerp(influence110, influence111, fade(lerpPointZ)), fade(lerpPointY)), fade(lerpPointX));

          // multiply by persistence value
          mData[i * mYsz + j + k * mXsz * mYsz] += lerpValue * std::pow(mPersistance, octi);
        }
      }
    }
  }
}

Noise3D::~Noise3D()
{
}

double Noise3D::operator()(std::size_t idx1, std::size_t idx2, std::size_t idx3) const
{
  return mData.at(idx1 * mYsz + idx2 + idx3 * mXsz * mYsz);
}

std::size_t Noise3D::seed() const
{
  return mSeed;
}

std::size_t Noise3D::xsz() const
{
  return mXsz;
}

std::size_t Noise3D::ysz() const
{
  return mYsz;
}

std::size_t Noise3D::zsz() const
{
  return mZsz;
}

std::size_t Noise3D::scale() const
{
  return mScale;
}

std::size_t Noise3D::octaves() const
{
  return mOctaves;
}

double Noise3D::lacunarity() const
{
  return mLacunarity;
}

double Noise3D::persistance() const
{
  return mPersistance;
}

std::vector<double> Noise3D::data() const
{
  return mData;
}