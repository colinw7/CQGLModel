#ifndef CQGL_MODEL_H
#define CQGL_MODEL_H

#include <CQMainWindow.h>

class CQMenu;
class CQMenuItem;
class CQGLControl;
class CQGLControlToolBar;
class CQGLCanvas;
class CGLModel;

class CQGLModel : public CQMainWindow {
  Q_OBJECT

 public:
  CQGLModel();

  CQGLControl *getControl() const { return control_; }

  CGLModel *getModel() const { return model_; }

  QWidget *createCentralWidget();

  void createWorkspace();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void createDockWindows();

  bool loadFile(const QString &filename);

 private slots:
  void controlSlot();
  void loadFileSlot();

 private:
  CQGLCanvas         *canvas_;
  CQGLControl        *control_;
  CQGLControlToolBar *toolbar_;
  CGLModel           *model_;

  CQMenu *fileMenu_;
  CQMenu *helpMenu_;
};

#endif
