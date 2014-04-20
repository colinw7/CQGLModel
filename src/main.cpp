#include <CQGLModel.h>
#include <QApplication>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQGLModel *model = new CQGLModel;

  model->init();

  model->show();

  if (argc > 1)
    model->loadFile(argv[1]);

  return app.exec();
}
