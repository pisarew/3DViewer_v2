//
// Created by Глеб Писарев on 26.02.2024.
//

#include "Controller.h"

s21::Controller::Controller(s21::Model& model) : model_(model) {}

void s21::Controller::LoadOBJ(const std::string& path) {
  model_.LoadObj(path);
  if (model_.Max() != 0) Scale(0.9f / model_.Max());
}
const s21::vertexes_type& s21::Controller::Vertexes() const {
  return model_.Vertexes();
}
const s21::facets_type& s21::Controller::Facets() const {
  return model_.Facets();
}
void s21::Controller::Scale(float factor) {
  model_.Scale(factor);
}
