#pragma once

#include <cstddef>

class Noise3D
{
private:
  std::size_t mSeed;
  std::size_t mXsz;
  std::size_t mYsz;
  std::size_t mZsz;
  std::size_t mScale;
  std::size_t mOctaves;
  double mLacunarity;
  double mPersistance;
  std::vector<double> mData;

  static double lerp(double a, double b, double t);
  static double fade(double t);

  // int loc(int x, int y, int z);
  // int sloc(int x, int y, int z, int scale);

public:
  Noise3D() = delete;
  Noise3D(std::size_t _seed, std::size_t _xsz, std::size_t _ysz, std::size_t _zsz);
  Noise3D(std::size_t _seed,    // seed
          std::size_t _xsz,     // x-direction size
          std::size_t _ysz,     // y-direction size
          std::size_t _zsz,     // z-direction size
          std::size_t _scale,   // scale
          std::size_t _octaves, // octaves level
          double _lacunarity,   // lacunarity
          double _persistance   // persistance
  );
  ~Noise3D();

  double operator()(std::size_t idx1, std::size_t idx2, std::size_t idx3) const;

  std::size_t seed() const;
  std::size_t xsz() const;
  std::size_t ysz() const;
  std::size_t zsz() const;
  std::size_t scale() const;
  std::size_t octaves() const;
  double lacunarity() const;
  double persistance() const;
  std::vector<double> data() const;
};