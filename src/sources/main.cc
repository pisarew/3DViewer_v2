//
// Created by Глеб Писарев on 26.02.2024.
//
#include <QApplication>

#include "Controller.h"
#include "MainView.h"
#include "Model.h"

int main(int argc, char *argv[]) {
  QSurfaceFormat format;
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);
  s21::Model model;
  s21::Controller controller(model);
  s21::MainView view(controller, model);
  view.show();
  return QApplication::exec();
}