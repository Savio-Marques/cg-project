#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <algorithm>

struct Vec3 {
    double x = 0, y = 0, z = 0;

    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }
    Vec3 operator-() const { return {-x, -y, -z}; }
    
    Vec3 hadamard(const Vec3& v) const { return {x * v.x, y * v.y, z * v.z}; }
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vec3 cross(const Vec3& v) const {
        return { y * v.z - z * v.y,
                 z * v.x - x * v.z,
                 x * v.y - y * v.x };
    }

    double norm() const { return std::sqrt(dot(*this)); }
    
    Vec3 normalize() const { 
        double n = norm(); 
        return n == 0 ? Vec3{0,0,0} : Vec3{x/n, y/n, z/n}; 
    }

    Vec3 clamp(double min = 0.0, double max = 1.0) const {
        return { std::max(min, std::min(max, x)), 
                 std::max(min, std::min(max, y)), 
                 std::max(min, std::min(max, z)) };
    }
};

#endif