#include <CGLModel.h>
#include <CImport3DS.h>
#include <CImport3DRW.h>
#include <CGeomImportASC.h>
#include <CGeomImportCOB.h>
#include <CImportDXF.h>
#include <CImportObj.h>
#include <CGeomImportPLG.h>
#include <CImportScene.h>
#include <CImportV3D.h>
#include <CImportX3D.h>
#include <CFile.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct CRGBAToFV {
  float fvalues[4];

  CRGBAToFV(const CRGBA &rgba) {
    fvalues[0] = rgba.getRed  ();
    fvalues[1] = rgba.getGreen();
    fvalues[2] = rgba.getBlue ();
    fvalues[3] = rgba.getAlpha();
  }
};

CGLModel::
CGLModel() :
 use_list_(true),
 list_num_(-1)
{
}

CGLModel::
~CGLModel()
{
}

bool
CGLModel::
import(CGeom3DType type, const std::string &filename)
{
  CFile file(filename);

  switch (type) {
    case CGEOM_3D_TYPE_3DRW: {
      CImport3DRW import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_3DS: {
      CImport3DS import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_ASC: {
      CGeomImportASC import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_COB: {
      CGeomImportCOB import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_DXF: {
      CImportDXF import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_OBJ: {
      CImportObj import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_PLG: {
      CGeomImportPLG import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_SCENE: {
      CImportScene import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_V3D: {
      CImportV3D import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    case CGEOM_3D_TYPE_X3D: {
      CImportX3D import;

      if (! import.read(file))
        return false;

      scene_ = import.releaseScene();

      break;
    }
    default:
      return false;
  }

  return true;
}

void
CGLModel::
addScene()
{
  if (scene_)
    addScene(*scene_);
}

void
CGLModel::
addScene(CGeomScene3D &scene)
{
  scene.getBBox(bbox_);

  CVector3D size   = bbox_.getSize();
  CPoint3D  center = bbox_.getCenter();

  CMatrix3D tmatrix1, smatrix, tmatrix2;

  double sx = 1.0/size.getX();
  double sy = 1.0/size.getY();
  double sz = 1.0/size.getZ();

  tmatrix1.setTranslation(-center.x, -center.y, -center.z);
  smatrix .setScale      (std::min(std::min(sx, sy), sz));
  tmatrix2.setTranslation(0, 0, 0);

  CMatrix3D matrix = tmatrix2*smatrix*tmatrix1;

  //------

  if (use_list_) {
    if (list_num_ != -1)
      glDeleteLists(list_num_, 1);

    list_num_ = glGenLists(1);

    glNewList(list_num_, GL_COMPILE);
  }

  const CGeomScene3D::ObjectList &objects = scene.getObjects();

  CGeomScene3D::ObjectList::const_iterator po1 = objects.begin();
  CGeomScene3D::ObjectList::const_iterator po2 = objects.end  ();

  for ( ; po1 != po2; ++po1)
    addObjectData(**po1, matrix);

  if (use_list_)
    glEndList();

  //------

#if 0
  if (use_list_)
    glNewList(getWireListNum(), GL_COMPILE);

  po1 = objects.begin();
  po2 = objects.end  ();

  for ( ; po1 != po2; ++po1)
    addWireframeObject(**po1, matrix);

  if (use_list_)
    glEndList();
#endif
}

void
CGLModel::
addObject(CGeomObject3D &object)
{
  object.getModelBBox(bbox_);

  CVector3D size   = bbox_.getSize();
  CPoint3D  center = bbox_.getCenter();

  CMatrix3D tmatrix1, smatrix, tmatrix2;

  double sx = 1.0/size.getX();
  double sy = 1.0/size.getY();
  double sz = 1.0/size.getZ();

  tmatrix1.setTranslation(-center.x, -center.y, -center.z);
  smatrix .setScale      (std::min(std::min(sx, sy), sz));
  tmatrix2.setTranslation(0, 0, 0);

  CMatrix3D matrix = tmatrix2*smatrix*tmatrix1;

  //----

  if (use_list_) {
    if (list_num_ != -1)
      glDeleteLists(list_num_, 1);

    list_num_ = glGenLists(1);

    glNewList(list_num_, GL_COMPILE);
  }

  addObjectData(object, matrix);

  if (use_list_)
    glEndList();
}

void
CGLModel::
addObjectData(CGeomObject3D &object, const CMatrix3D &matrix)
{
  glDisable(GL_COLOR_MATERIAL);

  glColor3d(0.5, 0.5, 0.5);

  CMatrix3D nmatrix = matrix.inverse().transposed();

  CMatrix3D vmatrix(CMATRIX_TYPE_IDENTITY);

  const CGeomObject3D::FaceList &faces = object.getFaces();

  CGeomObject3D::FaceList::const_iterator pf1 = faces.begin();
  CGeomObject3D::FaceList::const_iterator pf2 = faces.end  ();

  for ( ; pf1 != pf2; ++pf1) {
    const CGeomFace3D *face = *pf1;

    const CGeomFace3D::VertexList &vertices = face->getVertices();

    CGeomFace3D::VertexList::const_iterator pv1 = vertices.begin();
    CGeomFace3D::VertexList::const_iterator pv2 = vertices.end  ();

    for ( ; pv1 != pv2; ++pv1) {
      CGeomVertex3D &vertex = object.getVertex(*pv1);

      vertex.place(matrix);
      vertex.view (vmatrix);
    }

    const CRGBA &ambient   = face->getMaterial().getAmbient  ();
    const CRGBA &diffuse   = face->getMaterial().getDiffuse  ();
    const CRGBA &specular  = face->getMaterial().getSpecular ();
    double       shininess = face->getMaterial().getShininess();

    CVector3D normal;

    if (face->getNormalSet())
      normal = nmatrix*face->getNormal();
    else
      normal = object.verticesNormal(vertices);

    bool two_sided = face->getTwoSided();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, CRGBAToFV(ambient).fvalues);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CRGBAToFV(diffuse).fvalues);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, CRGBAToFV(specular).fvalues);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glColor3d(diffuse.getRed(), diffuse.getGreen(), diffuse.getBlue());

    if (! two_sided) {
      glBegin(GL_POLYGON);

      pv1 = vertices.begin();

      for ( ; pv1 != pv2; ++pv1) {
        const CGeomVertex3D &vertex = object.getVertex(*pv1);

        const CPoint3D &point = vertex.getViewed();

        if (vertex.getNormalSet()) {
          CVector3D normal = nmatrix*vertex.getNormal();

          glNormal3d(normal.getX(), normal.getY(), normal.getZ());
        }
        else
          glNormal3d(normal.getX(), normal.getY(), normal.getZ());

        glVertex3d(point.x, point.y, point.z);
      }

      glEnd();
    }
    else {
      glBegin(GL_POLYGON);

      pv1 = vertices.begin();

      for ( ; pv1 != pv2; ++pv1) {
        const CGeomVertex3D &vertex = object.getVertex(*pv1);

        const CPoint3D &point = vertex.getViewed();

        CVector3D normal1;

        if (vertex.getNormalSet())
          normal1 = nmatrix*vertex.getNormal();
        else
          normal1 = normal;

        glNormal3d(normal1.getX(), normal1.getY(), normal1.getZ());

        glVertex3d(point.x, point.y, point.z);
      }

      glEnd();

      glBegin(GL_POLYGON);

      CGeomFace3D::VertexList::const_reverse_iterator pv1 = vertices.rbegin();
      CGeomFace3D::VertexList::const_reverse_iterator pv2 = vertices.rend  ();

      for ( ; pv1 != pv2; ++pv1) {
        const CGeomVertex3D &vertex = object.getVertex(*pv1);

        const CPoint3D &point = vertex.getViewed();

        CVector3D normal1;

        if (vertex.getNormalSet())
          normal1 = nmatrix*vertex.getNormal();
        else
          normal1 = -1*normal;

        glNormal3d(normal1.getX(), normal1.getY(), normal1.getZ());

        glVertex3d(point.x, point.y, point.z);
      }

      glEnd();
    }
  }

  glEnable(GL_COLOR_MATERIAL);

  //-----

  const CGeomObject3D::LineList &lines = object.getLines();

  CGeomObject3D::LineList::const_iterator pl1 = lines.begin();
  CGeomObject3D::LineList::const_iterator pl2 = lines.end  ();

  for ( ; pl1 != pl2; ++pl1) {
    CGeomVertex3D vertex1 = (*pl1)->getStartVertex();
    CGeomVertex3D vertex2 = (*pl1)->getEndVertex  ();

    glBegin(GL_LINES);

    const CRGBA &color = (*pl1)->getMaterial().getDiffuse();

    vertex1.place(matrix);
    vertex1.view (vmatrix);

    vertex2.place(matrix);
    vertex2.view (vmatrix);

    const CPoint3D &point1 = vertex1.getViewed();
    const CPoint3D &point2 = vertex2.getViewed();

    glColor3d(color.getRed(), color.getGreen(), color.getBlue());

    glVertex3d(point1.x, point1.y, point1.z);
    glVertex3d(point2.x, point2.y, point2.z);

    glEnd();
  }

  GLenum err = glGetError();

  if (err != GL_NO_ERROR)
    std::cerr << "OpenGL Error: " << gluErrorString(err) << std::endl;
}

#if 0
void
CGLModel::
addWireframeObject(CGeomObject3D &object, const CMatrix3D &matrix)
{
  CMatrix3D vmatrix(CMATRIX_TYPE_IDENTITY);

  const CGeomObject3D::FaceList &faces = object.getFaces();

  CGeomObject3D::FaceList::const_iterator pf1 = faces.begin();
  CGeomObject3D::FaceList::const_iterator pf2 = faces.end  ();

  for ( ; pf1 != pf2; ++pf1) {
    const CGeomFace3D::VertexList &vertices = (*pf1)->getVertices();

    glBegin(GL_LINE_LOOP);

    const CRGBA &color = (*pf1)->getMaterial().getDiffuse();

    CGeomFace3D::VertexList::const_iterator pv1 = vertices.begin();
    CGeomFace3D::VertexList::const_iterator pv2 = vertices.end  ();

    for ( ; pv1 != pv2; ++pv1) {
      object.getVertex(*pv1).place(matrix);
      object.getVertex(*pv1).view(vmatrix);
    }

    //CVector3D normal = object.verticesNormal(vertices);

    pv1 = vertices.begin();

    for ( ; pv1 != pv2; ++pv1) {
      const CPoint3D &point = object.getVertex(*pv1).getViewed();

      glColor3d(color.getRed(), color.getGreen(), color.getBlue());

      //glNormal3d(normal.getX(), normal.getY(), normal.getZ());

      glVertex3d(point.x, point.y, point.z);
    }

    glEnd();
  }

  const CGeomObject3D::LineList &lines = object.getLines();

  CGeomObject3D::LineList::const_iterator pl1 = lines.begin();
  CGeomObject3D::LineList::const_iterator pl2 = lines.end  ();

  for ( ; pl1 != pl2; ++pl1) {
    CGeomVertex3D vertex1 = (*pl1)->getStartVertex();
    CGeomVertex3D vertex2 = (*pl1)->getEndVertex  ();

    glBegin(GL_LINES);

    const CRGBA &color = (*pl1)->getMaterial().getDiffuse();

    vertex1.place(matrix); vertex1.view(vmatrix);
    vertex2.place(matrix); vertex2.view(vmatrix);

    const CPoint3D &point1 = vertex1.getViewed();
    const CPoint3D &point2 = vertex2.getViewed();

    glColor3d(color.getRed(), color.getGreen(), color.getBlue());

    glVertex3d(point1.x, point1.y, point1.z);
    glVertex3d(point2.x, point2.y, point2.z);

    glEnd();
  }
}
#endif
