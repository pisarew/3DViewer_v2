//
// Created by Глеб Писарев on 05.02.2024.
//

#ifndef CPP4_3DVIEWER_V2_0_2_MODEL_H
#define CPP4_3DVIEWER_V2_0_2_MODEL_H

#include <string>
#include <vector>

namespace s21 {

/**
 * @brief Type alias for storing vertex coordinates.
 */
using vertexes_type = std::vector<float>;

/**
 * @brief Type alias for storing facet indices.
 */
using facets_type = std::vector<unsigned>;

/**
 * @brief Structure representing an object in 3D space with its vertices and
 * facets.
 */
struct Obj {
  vertexes_type vertexes; /**< Vector of vertex coordinates. */
  facets_type facets;     /**< Vector of facet indices. */
  float max;
};

/**
 * @brief Class for performing affine transformations on vertices.
 */
class Affine {
 public:
  /**
   * @brief Returns a reference to the singleton instance of Affine.
   * @return Reference to the singleton instance of Affine.
   */
  static Affine& GetInstance() noexcept;

  /**
   * @brief Scales the given vertices by the specified factor.
   * @param vertexes The vertices to scale.
   * @param scale The scaling factor.
   */
  static void Scale(vertexes_type& vertexes, float scale) noexcept;

  /**
   * @brief Rotates the given vertices around the specified rotation vector.
   * @param vertexes The vertices to rotate.
   * @param rotate The rotation vector specifying the rotation angles around x,
   * y, and z axes.
   */
  static void Rotate(vertexes_type& vertexes, vertexes_type rotate) noexcept;

  /**
   * @brief Moves the given vertices by the specified offset.
   * @param vertexes The vertices to move.
   * @param move The offset vector specifying the translation along x, y, and z
   * axes.
   */
  static void Move(vertexes_type& vertexes, const vertexes_type& move) noexcept;

 private:
  Affine(){}; /**< Private constructor to enforce singleton pattern. */
};

/**
 * @brief Class for loading OBJ files and creating Obj instances.
 */
class ObjLoader {
 public:
  /**
   * @brief Returns a reference to the singleton instance of ObjLoader.
   * @return Reference to the singleton instance of ObjLoader.
   */
  static ObjLoader& GetInstance() noexcept;

  /**
   * @brief Loads an OBJ file and returns an Obj instance representing the
   * loaded object.
   * @param path The path to the OBJ file.
   * @return Obj instance representing the loaded object.
   */
  static Obj Load(const std::string& path);

 private:
  ObjLoader(){}; /**< Private constructor to enforce singleton pattern. */

  /**
   * @brief Splits a string into substrings based on whitespace.
   * @param s The string to split.
   * @return Vector of substrings.
   */
  static std::vector<std::string> Split(const std::string& s) noexcept;

  /**
   * @brief Extracts the first number from a string.
   * @param s The string containing the number.
   * @return The extracted number.
   */
  static unsigned ExtractNumber(const std::string& s);
};

/**
 * @brief Class representing a 3D model with methods for loading OBJ files and
 * performing transformations.
 */

class Observer {
 public:
  virtual void Update() = 0;
};

class Observable {
 public:
  void AddObserver(Observer* observer);
  void NotifyObservers();

 private:
  std::vector<Observer*> observers_;
};

class Model: public Observable {
 public:
  /**
   * @brief Loads an OBJ file and populates the model with its data.
   * @param path The path to the OBJ file.
   */
  void LoadObj(const std::string& path);

  /**
   * @brief Scales the model by the specified factor.
   * @param factor The scaling factor.
   */
  void Scale(float factor) noexcept;

  /**
   * @brief Rotates the model around the specified rotation vector.
   * @param corner The rotation vector specifying the rotation angles around x,
   * y, and z axes.
   */
  void Rotate(const vertexes_type& corner) noexcept;

  /**
   * @brief Moves the model by the specified offset.
   * @param offset The offset vector specifying the translation along x, y, and
   * z axes.
   */
  void Move(const vertexes_type& offset) noexcept;

  /**
   * @brief Returns the vertex data of the model.
   * @return Const reference to the vertex data.
   */
  [[nodiscard]] const vertexes_type& Vertexes() const noexcept;

  /**
   * @brief Returns the facet data of the model.
   * @return Const reference to the facet data.
   */
  [[nodiscard]] const facets_type& Facets() const noexcept;

  [[nodiscard]] float Max() const noexcept;

  [[nodiscard]] bool Empty() const noexcept;

 private:
  Obj obj_; /**< The loaded OBJ data representing the model. */
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_2_MODEL_H
