//
// Created by Глеб Писарев on 26.02.2024.
//

#include "MainView.h"

#include <QFileDialog>

s21::MainView::MainView(s21::Controller& controller, s21::Model& model)
    : controller_(controller), model_(model), ui_(new Ui::MainView) {
  model_.AddObserver(this);
  ui_->setupUi(this);
}

s21::MainView::~MainView() { delete ui_; }

void s21::MainView::Update() {
  ui_->openGL->LoadDataToBuffers();
  ui_->openGL->update();
}

void s21::MainView::on_open_file_clicked() {
  const QString path = QFileDialog::getOpenFileName(this, "Выберите файл", "",
                                                    "Wavefront OBJ (*.obj)");
  if (!path.isEmpty()) OpenFile(path);
}
void s21::MainView::OpenFile(const QString& path) {
  controller_.LoadOBJ(path.toStdString());
  ui_->vertexesLabel->setText(
      "Вершины: " +
      QVariant((int)controller_.Vertexes().size() / 3).toString());
  ui_->edgesLabel->setText(
      "Вершины: " + QVariant((int)controller_.Facets().size() / 3).toString());
  ui_->openGL->SetVertexes(&controller_.Vertexes());
  ui_->openGL->SetFacets(&controller_.Facets());
  ui_->openGL->InitModelMatrix();
  Update();
}
void s21::MainView::on_plusButton_clicked() {
  controller_.Scale(1.15);
}
void s21::MainView::on_minusButton_clicked() {
  controller_.Scale(0.9);
}
