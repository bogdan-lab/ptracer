#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <utility>
#include <vector>

#include "Objects.h"

using ObjHolder = std::unique_ptr<Object>;
using ObjectCollection = std::vector<ObjHolder>;
/** Class that stores all objects in the modeled universe*/
class Scene {
 private:
  ObjectCollection scn_;

 public:
  Scene() = default;
  /** Adds given object to the universe*/
  Scene& AddObject(std::unique_ptr<Object> obj_ptr) {
    scn_.push_back(std::move(obj_ptr));
    return *this;
  }
  /** Returnes all object collection present in the simulated universe*/
  const ObjectCollection& GetObjects() const { return scn_; }
};

#endif  // SCENE_H
