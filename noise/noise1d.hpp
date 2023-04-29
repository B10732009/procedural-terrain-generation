#pragma once

#include <cstddef>

class Noise1D
{
private:
  std::size_t mSeed;
  std::size_t mX;
  double *mData;

  static double lerp(double a, double b, double t);
  static double fade(double t);

public:
  Noise1D() = delete;
  Noise1D(std::size_t seed, std::size_t x);
  Noise1D(std::size_t seed, std::size_t x, double scale, int octaves, double lacunarity, double persistance);
  ~Noise1D();

  double operator()(std::size_t t) const;

  std::size_t seed() const;
  std::size_t x() const;
  const double *data() const;
};