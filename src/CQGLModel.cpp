#include <CQGLModel.h>
#include <CQGLCanvas.h>
#include <CQMenu.h>
#include <CDir.h>
#include <CStrUtil.h>
#include <CGLModel.h>
#include <CQGLControl.h>

#include <QVBoxLayout>
#include <QFileDialog>

CQGLModel::
CQGLModel() :
 CQMainWindow("CQGLModel"),
 model_      (NULL)
{
}

QWidget *
CQGLModel::
createCentralWidget()
{
  QWidget *frame = new QWidget;

  QVBoxLayout *layout = new QVBoxLayout(frame);

  canvas_  = new CQGLCanvas(this);
  control_ = new CQGLControl(canvas_);

  toolbar_ = control_->createToolBar();

  connect(control_, SIGNAL(stateChanged()), this, SLOT(controlSlot()));

  layout->addWidget(toolbar_);
  layout->addWidget(canvas_);

  return frame;
}

void
CQGLModel::
createWorkspace()
{
}

void
CQGLModel::
createMenus()
{
  fileMenu_ = new CQMenu(this, "&File");

  CQMenuItem *loadFileItem = new CQMenuItem(fileMenu_, "Load File");

  connect(loadFileItem->getAction(), SIGNAL(triggered()), this, SLOT(loadFileSlot()));

  CQMenuItem *quitItem = new CQMenuItem(fileMenu_, "&Quit");

  quitItem->setShortcut("Ctrl+Q");
  quitItem->setStatusTip("Quit the application");

  connect(quitItem->getAction(), SIGNAL(triggered()), this, SLOT(close()));

  //----

  helpMenu_ = new CQMenu(this, "&Help");

  //----

  CQMenuItem *aboutItem = new CQMenuItem(helpMenu_, "&About");

  aboutItem->setStatusTip("Show the application's About box");

  //connect(aboutItem->getAction(), SIGNAL(triggered()), this, SLOT(aboutSlot()));
}

void
CQGLModel::
createToolBars()
{
}

void
CQGLModel::
createStatusBar()
{
}

void
CQGLModel::
createDockWindows()
{
}

void
CQGLModel::
controlSlot()
{
  canvas_->redraw();
}

void
CQGLModel::
loadFileSlot()
{
  QString title  = "Load Model";
  QString cwd    = QString(CDir::getCurrent().c_str());
  QString filter = "Model Files (*.3drw, *.3ds *.asc *.cob *.dxf *.obj *.plg *.scene *.v3d *.x3d)";

  QStringList filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext())
    loadFile(fi.next());

  canvas_->redraw();
}

bool
CQGLModel::
loadFile(const QString &filename)
{
  CFile file(filename.toStdString());

  if (! file.isRegular()) return false;

  delete model_;

  model_ = new CGLModel;

  std::string suffix = CStrUtil::toLower(file.getSuffix());

  if      (suffix == "3ds") {
    if (! model_->import(CGEOM_3D_TYPE_3DS, filename.toStdString()))
      return false;
  }
  else if (suffix == "asc") {
    if (! model_->import(CGEOM_3D_TYPE_ASC, filename.toStdString()))
      return false;
  }
  else if (suffix == "cob") {
    if (! model_->import(CGEOM_3D_TYPE_COB, filename.toStdString()))
      return false;
  }
  else if (suffix == "dxf") {
    if (! model_->import(CGEOM_3D_TYPE_DXF, filename.toStdString()))
      return false;
  }
  else if (suffix == "obj") {
    if (! model_->import(CGEOM_3D_TYPE_OBJ, filename.toStdString()))
      return false;
  }
  else if (suffix == "plg") {
    if (! model_->import(CGEOM_3D_TYPE_PLG, filename.toStdString()))
      return false;
  }
  else if (suffix == "scene") {
    if (! model_->import(CGEOM_3D_TYPE_SCENE, filename.toStdString()))
      return false;
  }
  else if (suffix == "v3d") {
    if (! model_->import(CGEOM_3D_TYPE_V3D, filename.toStdString()))
      return false;
  }
  else if (suffix == "x3d") {
    if (! model_->import(CGEOM_3D_TYPE_X3D, filename.toStdString()))
      return false;
  }
  else
    return false;

  model_->addScene();

  return true;
}
