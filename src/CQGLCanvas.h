#ifndef CQGL_CANVAS_H
#define CQGL_CANVAS_H

#include <QGLWidget>

class CQGLModel;

class CQGLCanvas : public QGLWidget {
  Q_OBJECT

 public:
  CQGLCanvas(CQGLModel *mode);
 ~CQGLCanvas();

  void redraw();

 protected:
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void paintGL();

  void resizeGL(int width, int height);

  void mousePressEvent  (QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent   (QMouseEvent *event);

  void keyPressEvent(QKeyEvent *event);

 private:
  CQGLModel *model_;
};

#endif
