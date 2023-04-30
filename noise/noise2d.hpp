#pragma once

#include <cstddef>

class Noise2D
{
private:
  std::size_t mSeed;
  std::size_t mX;
  std::size_t mY;
  double *mData;

  static double lerp(double a, double b, double t);
  static double fade(double t);

public:
  Noise2D() = delete;
  Noise2D(std::size_t seed, std::size_t x, std::size_t y);
  Noise2D(std::size_t seed, std::size_t x, std::size_t y, double scale, int octaves, double lacunarity,
          double persistance);
  ~Noise2D();

  double operator()(std::size_t t1, std::size_t t2) const;

  std::size_t seed() const;
  std::size_t x() const;
  std::size_t y() const;
  const double *data() const;
};