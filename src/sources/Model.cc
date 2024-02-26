//
// Created by Глеб Писарев on 05.02.2024.
//

#include "Model.h"

#include <cmath>
#include <fstream>
#include <regex>
#include <sstream>

void s21::Model::LoadObj(const std::string& path) {
  obj_ = ObjLoader::GetInstance().Load(path);
}

const s21::vertexes_type& s21::Model::Vertexes() const noexcept {
  return obj_.vertexes;
}
const s21::facets_type& s21::Model::Facets() const noexcept {
  return obj_.facets;
}

void s21::Model::Scale(float factor) noexcept {
  Affine::GetInstance().Scale(obj_.vertexes, factor);
  NotifyObservers();
}
void s21::Model::Rotate(const s21::vertexes_type& corner) noexcept {
  Affine::GetInstance().Rotate(obj_.vertexes, corner);
  NotifyObservers();
}
void s21::Model::Move(const s21::vertexes_type& offset) noexcept {
  Affine::GetInstance().Move(obj_.vertexes, offset);
  NotifyObservers();
}
float s21::Model::Max() const noexcept { return obj_.max; }
bool s21::Model::Empty() const noexcept {
  return obj_.facets.empty() || obj_.vertexes.empty();
}

s21::ObjLoader& s21::ObjLoader::GetInstance() noexcept {
  static ObjLoader instance;
  return instance;
}
s21::Obj s21::ObjLoader::Load(const std::string& path) {
  Obj obj;
  obj.max = 0;
  std::ifstream file;
  file.open(path);
  if (!file.is_open()) throw std::runtime_error("Opening error");
  std::regex vertex_pattern(R"(\s*v\s+([\d\.-]+)\s+([\d\.-]+)\s+([\d\.-]+))");

  std::string line;
  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_match(line, match, vertex_pattern)) {
      for (size_t i = 1; i < match.size(); ++i) {
        auto number = std::stof(match[i]);
        if (abs(number) > abs(obj.max)) obj.max = number;
        obj.vertexes.push_back(number);
      }
    } else if (line[0] == 'f') {
      auto str_facets = Split(line);
      facets_type numbers;
      for (const auto& item : str_facets) {
        try {
          numbers.push_back(ExtractNumber(item) - 1);
        } catch (...) {
        }
      }
      for (size_t i = 1; i < numbers.size(); ++i) {
        obj.facets.push_back(numbers[i - 1]);
        obj.facets.push_back(numbers[i]);
      }
      if (!numbers.empty()) {
        obj.facets.push_back(numbers[numbers.size() - 1]);
        obj.facets.push_back(numbers[0]);
      }
    }
  }
  return obj;
}
std::vector<std::string> s21::ObjLoader::Split(const std::string& s) noexcept {
  std::vector<std::string> result;
  std::istringstream iss(s);
  std::string word;

  while (iss >> word) {
    result.push_back(word);
  }

  return result;
}
unsigned s21::ObjLoader::ExtractNumber(const std::string& s) {
  size_t pos = s.find_first_of('/');
  std::string numberStr = s.substr(0, pos);

  if (numberStr.empty()) {
    throw std::invalid_argument(
        "No number found at the beginning of the string");
  }

  return std::stoi(numberStr);
}

s21::Affine& s21::Affine::GetInstance() noexcept {
  static Affine instance;
  return instance;
}

void s21::Affine::Scale(s21::vertexes_type& vertexes, float scale) noexcept {
  if (scale == 0) return;
  for (float& item : vertexes) item = item * scale;
}
void s21::Affine::Rotate(s21::vertexes_type& vertexes,
                         s21::vertexes_type rotate) noexcept {
  if (vertexes.empty()) return;
  for (size_t i = 0; i < 3; i++) rotate[i] = rotate[i] * (float)M_PI / 180;
  if (vertexes.size() % 3 == 0) {
    float v_temp[3];
    for (size_t i = 0; i <= vertexes.size(); i += 3) {
      for (size_t j = 0; j < 3; j++) v_temp[j] = vertexes[i + j];
      vertexes[i + 1] = v_temp[1] * cos(rotate[0]) - v_temp[2] * sin(rotate[0]);
      vertexes[i + 2] = v_temp[1] * sin(rotate[0]) + v_temp[2] * cos(rotate[0]);
      for (size_t j = 0; j < 3; j++) v_temp[j] = vertexes[i + j];
      vertexes[i] = v_temp[0] * cos(rotate[1]) + v_temp[2] * sin(rotate[1]);
      vertexes[i + 2] =
          -v_temp[0] * sin(rotate[1]) + v_temp[2] * cos(rotate[1]);
      for (size_t j = 0; j < 3; j++) v_temp[j] = vertexes[i + j];
      vertexes[i] = v_temp[0] * cos(rotate[2]) - v_temp[1] * sin(rotate[2]);
      vertexes[i + 1] = v_temp[0] * sin(rotate[2]) + v_temp[1] * cos(rotate[2]);
    }
  }
}
void s21::Affine::Move(s21::vertexes_type& vertexes,
                       const s21::vertexes_type& move) noexcept {
  if (vertexes.empty()) return;
  if (vertexes.size() % 3 == 0) {
    for (size_t i = 0; i <= vertexes.size(); i++) {
      vertexes[i] = vertexes[i] + move[i % 3];
    }
  }
}

void s21::Observable::AddObserver(Observer* observer) {
  observers_.push_back(observer);
}
void s21::Observable::NotifyObservers() {
  for (auto const& observer : observers_) {
    observer->Update();
  }
}