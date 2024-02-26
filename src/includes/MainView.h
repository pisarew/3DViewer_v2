//
// Created by Глеб Писарев on 26.02.2024.
//

#ifndef INC_3DVIEWER_V2_MAINVIEW_H
#define INC_3DVIEWER_V2_MAINVIEW_H

#include <QMainWindow>

#include "Controller.h"
#include "Model.h"
#include "ui_MainView.h"

namespace s21 {
class MainView : public QMainWindow, public Observer {
  Q_OBJECT

 public:
  MainView(Controller& controller, Model& model);
  ~MainView() override;

 private slots:
  void on_open_file_clicked();
  void on_plusButton_clicked();
  void on_minusButton_clicked();

 private:
  Ui::MainView* ui_;
  Controller& controller_;
  Model& model_;

  void Update() override;

  void OpenFile(const QString& path);
};
}  // namespace s21

#endif  // INC_3DVIEWER_V2_MAINVIEW_H
