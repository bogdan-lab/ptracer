#include<gtest/gtest.h>

#include<Camera.h>


TEST (CameraTests, Creation) {
#ifndef NDEBUG
    {
        auto get_cam = [](double w, double h, double d){return Camera{w,h,d};};
        EXPECT_DEATH(get_cam(600,400,0 ), "");
        EXPECT_DEATH(get_cam(600,0,100 ), "");
        EXPECT_DEATH(get_cam(0,400,100 ), "");
        EXPECT_DEATH(get_cam(600,400,-1), "");
        EXPECT_DEATH(get_cam(600,-1,100), "");
        EXPECT_DEATH(get_cam(-1,400,100), "");
    }
#endif //NDEBUG
    {
        Camera cam{600, 400, 50};
        GeoVec exp{300,200,-50};
        EXPECT_EQ(cam.GetCamPos(), exp);
    }
}


TEST (CameraTests, MakeCameraRay) {
    Camera cam{600,400,4};
    cam.SetRSmooth(0);
    {
        Ray r = cam.MakeCameraRay(0, 0);
        GeoVec exp_pos{300, 200, -4};
        GeoVec exp_dir{exp_pos, GeoVec{0,0,0}};
        exp_dir.Norm();
        EXPECT_EQ(exp_pos, r.GetPos());
        EXPECT_EQ(exp_dir, r.GetDir());
    }
    {
        Ray r = cam.MakeCameraRay(300,200);
        GeoVec exp_pos{300, 200, -4};
        GeoVec exp_dir{exp_pos, GeoVec{300,200,0}};
        exp_dir.Norm();
        EXPECT_EQ(exp_pos, r.GetPos());
        EXPECT_EQ(exp_dir, r.GetDir());

    }
    cam.SetRSmooth(3).SetSeed(42);
    {
        for(size_t i=0; i<1000; i++){
            Ray r = cam.MakeCameraRay(300, 200);
            EXPECT_GE(r.GetDir().z_, 0.8);
        }
    }
}


TEST (CameraTests, CreatePixel) {
    Camera cam{600,400,4};
    cam.SetRSmooth(0);
    Camera::SetSamplePerPixel(100);
#ifndef NDEBUG
        EXPECT_DEATH(Camera::SetSamplePerPixel(0), "");
#endif //NDEBUG
    {
        Pixel px = cam.CreatePixel(125, 64);
        const auto& r_begin = px.GetPixelRays().front();
        for (const auto& r : px.GetPixelRays()) {
            EXPECT_EQ(r_begin.GetDir(), r.GetDir());
            EXPECT_EQ(r_begin.GetPos(), r.GetPos());
        }
    }
    cam.SetRSmooth(3).SetSeed(42);
    {
        Pixel px = cam.CreatePixel(300, 200);
        const auto& all_rays = px.GetPixelRays();
        for (size_t i=1; i<all_rays.size(); i++) {
            EXPECT_NE(all_rays[i-1].GetDir(), all_rays[i].GetDir());
            EXPECT_NE(all_rays[i-1].GetPos(), all_rays[i].GetPos());
        }
    }
}



