#pragma once

#include <cstddef>
#include <vector>

class Noise1D
{
private:
  std::size_t mSeed;
  std::size_t mXsz;
  std::size_t mScale;
  std::size_t mOctaves;
  double mLacunarity;
  double mPersistance;
  std::vector<double> mData;

  static double lerp(double a, double b, double t);
  static double fade(double t);

public:
  Noise1D() = delete;
  Noise1D(std::size_t _seed, std::size_t _xsz);
  Noise1D(std::size_t _seed,    // seed
          std::size_t _xsz,     // x-direction size
          std::size_t _scale,   // scale
          std::size_t _octaves, // octaves level
          double _lacunarity,   // lacunarity
          double _persistance   // persistance
  );
  ~Noise1D();

  double operator()(std::size_t idx) const;

  std::size_t seed() const;
  std::size_t xsz() const;
  std::size_t scale() const;
  std::size_t octaves() const;
  double lacunarity() const;
  double persistance() const;
  std::vector<double> data() const;
};