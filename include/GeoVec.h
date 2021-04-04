﻿#ifndef GEO_VEC_H
#define GEO_VEC_H

#include<cmath>


struct GeoVec{
    double x_, y_, z_;

    GeoVec() = delete;
    GeoVec(double x, double y, double z): x_(x), y_(y), z_(z) {}
    GeoVec(const GeoVec& start, const GeoVec& end) {
        x_ = end.x_ - start.x_;
        y_ = end.y_ - start.y_;
        z_ = end.z_ - start.z_;
    }

    double Len() const {return std::sqrt(x_*x_ + y_*y_ + z_*z_);}

    GeoVec& Norm() {
        double l = Len();
        x_ /= l;
        y_ /= l;
        z_ /= l;
        return *this;
    }

    double Dot(const GeoVec& other) const {
        return other.x_*x_ + other.y_*y_ + other.z_*z_;
    }

    GeoVec Cross(const GeoVec& other) const {
        return { y_*other.z_ - z_*other.y_,
                 z_*other.x_ - x_*other.z_,
                 x_*other.y_ - y_*other.x_ };
    }

};

inline GeoVec operator/(const GeoVec& lhs, double rhs){
    return {lhs.x_/rhs, lhs.y_/rhs, lhs.z_/rhs};
}

inline GeoVec operator*(const GeoVec& lhs, double rhs){
    return {lhs.x_*rhs, lhs.y_*rhs, lhs.z_*rhs};
}

inline GeoVec operator+(const GeoVec& lhs, const GeoVec& rhs){
    return {lhs.x_+rhs.x_, lhs.y_+rhs.y_, lhs.z_+rhs.z_};
}

inline GeoVec operator-(const GeoVec& lhs, const GeoVec& rhs){
    return {lhs.x_-rhs.x_, lhs.y_-rhs.y_, lhs.z_-rhs.z_};
}



#endif 	//GEO_VEC_H