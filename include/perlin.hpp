#pragma once

class Perlin
{
  public:
    Perlin()
    {
      for (int i = 0; i < point_count; i++)
      {
        random_vector[i] = unit_vector(Vector3::random(-1, 1));
      }

      perlinGeneratePerm(perm_x);
      perlinGeneratePerm(perm_y);
      perlinGeneratePerm(perm_z);
    }

    double noise(const Point3& point) const
    {
      double u = point.x() - std::floor(point.x());
      double v = point.y() - std::floor(point.y());
      double w = point.z() - std::floor(point.z());

      int i = int(std::floor(point.x()));
      int j = int(std::floor(point.y()));
      int k = int(std::floor(point.z()));

      Vector3 c[2][2][2];

      for(int di=0; di < 2; di++)
        for(int dj=0; dj < 2; dj++)
          for(int dk=0; dk < 2; dk++)
            c[di][dj][dk] = random_vector[
              perm_x[(i+di) & 255] ^
              perm_y[(j+dj) & 255] ^
              perm_z[(k+dk) & 255] 
            ];

      return perlinInterpolation(c, u, v, w);
    }

    double turbulence(const Point3& point, int depth) const
    {
      double accum = 0.0;
      Point3 temp_point = point;
      double weight = 1.0;

      for (int i = 0; i < depth; i++)
      {
        accum += weight * noise(temp_point);
        weight *= 0.5;
        temp_point *= 2.0;
      }
      return std::fabs(accum);      
    }

  private:
    static const int point_count = 256;
    Vector3 random_vector[point_count];
    int perm_x[point_count];
    int perm_y[point_count];
    int perm_z[point_count];

    static void perlinGeneratePerm(int* p)
    {
      for (int i = 0; i < point_count; i++)
      {
        p[i] = i;
      }

      permute(p, point_count);
    }

    static void permute(int* p, int n)
    {
      for (int i = n-1; i > 0; i--)
      {
        int target = randomInt(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
      }
    }

    static double perlinInterpolation(const Vector3 c[2][2][2], double u, double v, double w)
    {
      auto uu = u*u*(3-2*u);
      auto vv = v*v*(3-2*v);
      auto ww = w*w*(3-2*w);

      auto accum = 0.0;
      
      for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
          for (int k = 0; k < 2; k++)
          {
            Vector3 weight_v(u-i, v-j, w-k);
            accum += (i*uu + (1-i)*(1-uu))
                   * (j*vv + (1-j)*(1-vv))
                   * (k*ww + (1-k)*(1-ww))
                   * dot(c[i][j][k], weight_v);
          }
      return accum;
    }
};

/*
* TODO : fix the perlin noise to get the same result as in the book.
*/