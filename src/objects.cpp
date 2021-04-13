#include "Objects.h"
#include <optional>
#include <cmath>

std::optional<double> Sphere::GetClosesDist(const Ray& ray) const {
    GeoVec ray_to_c {ray.GetPos(), center_};
    double dist_to_c = ray_to_c.Len();
    assert(dist_to_c>r_); 	 	//should not be inside sphere
    double dist_proj = ray_to_c.Dot(ray.GetDir());
    if (dist_proj<=0) return std::nullopt; 	//ray directed away!
    double D_quarter = dist_proj*dist_proj - dist_to_c*dist_to_c + r_*r_;
    if(D_quarter<=0) return std::nullopt; 	//==0 - ignore when ray only touch the surface
    return dist_proj - std::sqrt(D_quarter);
}



std::optional<double> Triangle::GetClosesDist(const Ray &ray) const {
    //find ray crossection with the plain
    const auto& ray_dir = ray.GetDir();
    const auto& ray_pos = ray.GetPos();
    double den = ray_dir.Dot(norm_);
    if (den<=0) return std::nullopt; 	//ray is directed away!
    double num = ray_pos.Dot(norm_);
    double t = -(D_ + num)/den;
    GeoVec cross_pt = ray_pos + t*ray_dir;
    //check if point inside triangle
    GeoVec to_p0{cross_pt, p0_};
    GeoVec to_p1{cross_pt, p1_};
    GeoVec to_p2{cross_pt, p2_};
    GeoVec v0 = to_p0.Cross(to_p1);
    GeoVec v1 = to_p1.Cross(to_p2);
    GeoVec v2 = to_p2.Cross(to_p2);
    if(v0.Dot(norm_)<=0 || v1.Dot(norm_)<=0 || v2.Dot(norm_)) return std::nullopt;
    //calculate distance
    return dist_btw_points(ray_pos, cross_pt);
}
