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