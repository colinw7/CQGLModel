TEMPLATE = app

TARGET = CQGLModel

DEPENDPATH += .

QT += opengl

CONFIG += debug

# Input
SOURCES += \
CGeomAxes3D.cpp \
CGeomBox3D.cpp \
CGeomCamera3D.cpp \
CGeomCone3D.cpp \
CGeomCube3D.cpp \
CGeomCylinder3D.cpp \
CGeometry3D.cpp \
CGeomFace3D.cpp \
CGeomFill3D.cpp \
CGeomImportASC.cpp \
CGeomImportCOB.cpp \
CGeomImportPLG.cpp \
CGeomLight3D.cpp \
CGeomLine3D.cpp \
CGeomObject3D.cpp \
CGeomParticle3D.cpp \
CGeomPoint3D.cpp \
CGeomPyramid3D.cpp \
CGeomScene3D.cpp \
CGeomSphere3D.cpp \
CGeomTorus3D.cpp \
CGeomTriangle3D.cpp \
CGeomUtil3D.cpp \
CGeomVertex3D.cpp \
CGLModel.cpp \
CGLUtil.cpp \
CGLTexture.cpp \
CImport3DRW.cpp \
CImport3DS.cpp \
CImportASC.cpp \
CImportCOB.cpp \
CImportDXF.cpp \
CImportObj.cpp \
CImportPLG.cpp \
CImportScene.cpp \
CImportV3D.cpp \
CImportX3D.cpp \
CQGLCanvas.cpp \
CQGLControl.cpp \
CMathGeom2D.cpp \
CMathGeom3D.cpp \
CArcToBezier.cpp \
CQGLModel.cpp \
main.cpp \

HEADERS += \
CClipSide.h \
CDrawType3D.h \
CGeom3DRenderer.h \
CGeom3DType.h \
CGeomAxes3D.h \
CGeomBox3D.h \
CGeomCamera3D.h \
CGeomCone3D.h \
CGeomCube3D.h \
CGeomCylinder3D.h \
CGeometry3D.h \
CGeomFace3D.h \
CGeomFill3D.h \
CGeomImportASC.h \
CGeomImportCOB.h \
CGeomImportPLG.h \
CGeomLight3D.h \
CGeomLine3D.h \
CGeomMask.h \
CGeomObject3D.h \
CGeomParticle3D.h \
CGeomPoint3D.h \
CGeomPyramid3D.h \
CGeomScene3D.h \
CGeomSphere3D.h \
CGeomTexture.h \
CGeomTorus3D.h \
CGeomTriangle3D.h \
CGeomUtil3D.h \
CGeomVertex3D.h \
CGeomZBuffer.h \
CGLModel.h \
CGLTexture.h \
CImport3DRW.h \
CImport3DS.h \
CImportASC.h \
CImportCOB.h \
CImportDXFColors.h \
CImportDXF.h \
CImportObj.h \
CImportPLG.h \
CImportScene.h \
CImportV3D.h \
CImportX3D.h \
CMaterial.h \
CQGLCanvas.h \
CQGLControl.h \
CQGLModel.h \
CMathGeom2D.h \
CArcToBezier.h \
CShadeType3D.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFile/include \
../../COS/include \
../../CMath/include \
../../CRegExp/include \
../../CFont/include \
../../CStrUtil/include \
../../CRGBName/include \
../../CUtil/include \
.

unix:LIBS += \
-L$$LIB_DIR \
-L../../lib \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCQUtil -lCImageLib -lCFile -lCOS -lCStrUtil -lCRegExp -lCRGBName \
-lglut -lGLU -lGL -lpng -ljpeg -ltre
