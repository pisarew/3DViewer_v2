//
// Created by Глеб Писарев on 26.02.2024.
//

#ifndef INC_3DVIEWER_V2_OPENGLWIDGET_H
#define INC_3DVIEWER_V2_OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QtCore>
#include <QtOpenGL>
#include <vector>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  explicit OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget() override;
  [[nodiscard]] bool IsLines() const;
  [[nodiscard]] bool IsPoints() const;
  void SetVertexes(const std::vector<GLfloat>* vertexes);
  void SetFacets(const std::vector<unsigned>* facets);
  void LoadDataToBuffers();
  void InitModelMatrix();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* mouse) override;

 private:
  GLuint shader_program_;
  GLuint VAO, VBO, EBO;
  bool is_data_load_ = false;
  QPoint mouse_position_;
  QMatrix4x4 model_matrix_;
  const std::vector<GLfloat>* vertexes_ = nullptr;
  const std::vector<unsigned>* facets_ = nullptr;

  static std::optional<std::string> GetShaderSource(
      const std::string& filename);
  void InitBuffers();
  void InitShaderProgram();
  void RotateCoordinateSystem(float angle, const QVector3D& axis);


};

#endif  // INC_3DVIEWER_V2_OPENGLWIDGET_H
