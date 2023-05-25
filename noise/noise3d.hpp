#pragma once

#include <cstddef>

class Noise3D
{
private:
  std::size_t mSeed;
  std::size_t mX;
  std::size_t mY;
  std::size_t mZ;
  double *mData;

  static double lerp(double a, double b, double t);
  static double fade(double t);
  int loc(int x, int y, int z);
  int sloc(int x, int y, int z, int scale);

public:
  Noise3D() = delete;
  Noise3D(std::size_t seed, std::size_t x, std::size_t y, std::size_t z);
  Noise3D(std::size_t seed, std::size_t x, std::size_t y, std::size_t z, double scale, int octaves, double lacunarity,
          double persistance);
  ~Noise3D();

  double operator()(std::size_t t1, std::size_t t2, std::size_t t3) const;

  std::size_t seed() const;
  std::size_t x() const;
  std::size_t y() const;
  std::size_t z() const;
  const double *data() const;
};