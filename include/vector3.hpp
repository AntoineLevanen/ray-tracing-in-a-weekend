#pragma once

class Vector3
{
  public:
  double e[3];

  Vector3() : e{0, 0, 0}{}
  Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const {return e[0];}
  double y() const {return e[1];}
  double z() const {return e[2];}
  
  Vector3 operator-() const {return Vector3(-e[0], -e[1], -e[2]);}
  double operator[](int i) const {return e[i];}
  double& operator[](int i) {return e[i];}

  Vector3& operator+=(const Vector3& v)
  {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vector3& operator*=(double t)
  {
    e[0] += t;
    e[1] += t;
    e[2] += t;
    return *this;
  }

  Vector3& operator/=(double t)
  {
    return *this *= 1/t;
  }

  double lenght() const 
  {
    return std::sqrt(length_squared());
  }

  double length_squared() const
  {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  bool nearZero() const
  {
    // Return true if the vector is close to zero in all dimentions.
    double s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
  }

  static Vector3 random()
  {
    return Vector3(randomDouble(), randomDouble(), randomDouble());
  }

  static Vector3 random(double min, double max)
  {
    return Vector3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
  }
};

using Point3 = Vector3;

inline std::ostream& operator<<(std::ostream& out ,const Vector3& v)
{
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2]; 
}

inline Vector3 operator+(const Vector3 u, const Vector3 v)
{
  return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 u, const Vector3 v)
{
  return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
  return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3 v)
{
  return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, double t)
{
  return t * v;
}

inline Vector3 operator/(const Vector3 v, double t)
{
  return (1/t) * v;
}

inline double dot(const Vector3& u, const Vector3& v)
{
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2]; 
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
  return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                 u.e[2] * v.e[0] - u.e[0] * v.e[2],
                 u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(const Vector3& v)
{
  return v / v.lenght();
}

inline Vector3 randomUnitVector()
{
  // Find a good vector candidate.
  // Only have MAX_ITER try before giving up to avoid being stuck here.
  int nb_iter = 0;
  int MAX_ITER = 250;
  while(nb_iter <= MAX_ITER)
  {
    nb_iter++;

    Vector3 random_vector_candidate = Vector3::random(-1, 1);
    double candidate_length_squared = random_vector_candidate.length_squared();
    if (1e-160 < candidate_length_squared && candidate_length_squared <= 1.0)
    {
      return random_vector_candidate / sqrt(candidate_length_squared);
    }
  }
  return Vector3(0.0, 0.0, 0.0);
}

inline Vector3 randomOnHemisphere(const Vector3& normal)
{
  Vector3 on_unit_sphere = randomUnitVector();
  if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
  {
    return on_unit_sphere;
  }
  else
  {
    return -on_unit_sphere;
  }
}

inline Vector3 randomInUnitDisk()
{
  // TODO add a counter to clamp the max iteration number on the while(true)
  while(true)
  {
    auto p = Vector3(randomDouble(-1, 1), randomDouble(-1, 1), 0.0);
    if(p.length_squared() < 1)
    {
      return p;
    }
  }
}

inline Vector3 reflect(const Vector3& incoming_ray, const Vector3& normal)
{
  return incoming_ray - 2 * dot(incoming_ray, normal) * normal;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat)
{
  auto cos_theta = std::fmin(dot(-uv, n), 1.0);
  Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vector3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

