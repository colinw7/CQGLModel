#ifndef CGL_MODEL_H
#define CGL_MODEL_H

#include <accessor.h>

#include <CRGBA.h>
#include <CMatrix3D.h>
#include <CBBox3D.h>
#include <CGeom3DType.h>
#include <CAutoPtr.h>

class CGeomScene3D;
class CGeomObject3D;
class CKeyEvent;

class CGLModel {
 private:
  CAutoPtr<CGeomScene3D> scene_;
  bool                   use_list_;
  int                    list_num_;
  CBBox3D                bbox_;

 public:
  CGLModel();
 ~CGLModel();

  ACCESSOR(UseList, bool, use_list)
  ACCESSOR(ListNum, int , list_num)

  CGeomScene3D *getScene() const { return scene_; }

  bool import(CGeom3DType type, const std::string &filename);

  void addScene();

 private:
  void addScene(CGeomScene3D &scene);

  void addObject(CGeomObject3D &object);

  void addObjectData(CGeomObject3D &object, const CMatrix3D &matrix);
};

#endif
