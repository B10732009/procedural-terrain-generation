#pragma once

#include <cstddef>
#include <vector>

class Noise2D
{
private:
  std::size_t mSeed;         // seed
  std::size_t mXsz;          // x-direction size
  std::size_t mYsz;          // y-direction size
  std::size_t mScale;        // scale
  std::size_t mOctaves;      // octaves level
  double mLacunarity;        // lacunarity
  double mPersistance;       // persistance
  std::vector<double> mData; // noise values

  static double lerp(double a, double b, double t);
  static double fade(double t);

public:
  Noise2D() = delete;
  Noise2D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz);
  Noise2D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz, std::size_t _scale, std::size_t _octaves, double _lacunarity, double _persistance);
  ~Noise2D();

  double operator()(std::size_t idx1, std::size_t idx2) const;

  std::size_t seed() const;
  std::size_t xsz() const;
  std::size_t ysz() const;
  std::size_t scale() const;
  std::size_t octaves() const;
  double lacunarity() const;
  double persistance() const;
  std::vector<double> data() const;
};