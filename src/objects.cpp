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
