//
// Created by Глеб Писарев on 26.02.2024.
//

#ifndef INC_3DVIEWER_V2_CONTROLLER_H
#define INC_3DVIEWER_V2_CONTROLLER_H

#include <string>

#include "Model.h"

namespace s21 {
class Controller {
 public:
  explicit Controller(Model& model);

  void LoadOBJ(const std::string& path);
  void Scale(float factor);

  [[nodiscard]] const vertexes_type& Vertexes() const;
  [[nodiscard]] const facets_type& Facets() const;
 private:
  Model& model_;
};
}  // namespace s21
#endif  // INC_3DVIEWER_V2_CONTROLLER_H
