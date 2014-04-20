#include <CQGLCanvas.h>
#include <CQGLModel.h>
#include <CQUtil.h>
#include <CQGLControl.h>
#include <CGLModel.h>

struct CRGBAToFV {
  float fvalues[4];

  CRGBAToFV(const CRGBA &rgba) {
    fvalues[0] = rgba.getRed  ();
    fvalues[1] = rgba.getGreen();
    fvalues[2] = rgba.getBlue ();
    fvalues[3] = rgba.getAlpha();
  }
};

CQGLCanvas::
CQGLCanvas(CQGLModel *model) :
 QGLWidget(model), model_(model)
{
  setFocusPolicy(Qt::StrongFocus);

  //setDisableRedraw(true);
}

CQGLCanvas::
~CQGLCanvas()
{
}

QSize
CQGLCanvas::
minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize
CQGLCanvas::
sizeHint() const
{
  return QSize(600, 600);
}

void
CQGLCanvas::
redraw()
{
  update();
}

void
CQGLCanvas::
paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  CQGLControl *control = model_->getControl();

  control->updateGL();

  CGLModel *model = model_->getModel();

  if (model)
    glCallList(model->getListNum());
}

void
CQGLCanvas::
resizeGL(int width, int height)
{
  CQGLControl *control = model_->getControl();

  control->handleResize(width, height);

  //----

  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  CRGBA ambient(0.4, 0.4, 0.4);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CRGBAToFV(ambient).fvalues);

  glEnable(GL_NORMALIZE);

  static GLfloat pos[4] = {100.0, 100.0, 100.0, 0.0};

  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  CRGBA diffuse(0.6, 0.2, 0.2);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, CRGBAToFV(diffuse).fvalues);

  glEnable(GL_LIGHT0);
}

void
CQGLCanvas::
mousePressEvent(QMouseEvent *e)
{
  CQGLControl *control = model_->getControl();

  control->handleMousePress(e);

  update();
}

void
CQGLCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CQGLControl *control = model_->getControl();

  control->handleMouseRelease(e);

  update();
}

void
CQGLCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CQGLControl *control = model_->getControl();

  control->handleMouseMotion(e);

  update();
}

void
CQGLCanvas::
keyPressEvent(QKeyEvent *)
{
}
