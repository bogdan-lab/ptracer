#ifndef SCENE_H
#define SCENE_H

#include "Objects.h"

#include <vector>
#include <memory>
#include <utility>

using ObjHolder =  std::unique_ptr<Object>;
using ObjectCollection = std::vector<ObjHolder>;

class Scene {
private:
    ObjectCollection scn_;

public:
    Scene() = default;

    Scene& AddObject(std::unique_ptr<Object> obj_ptr) {
        scn_.push_back(std::move(obj_ptr));
        return *this;
    }

    const ObjectCollection& GetObjects() const { return scn_; }

};



#endif //SCENE_H
