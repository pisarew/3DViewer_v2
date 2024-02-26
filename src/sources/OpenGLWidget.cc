//
// Created by Глеб Писарев on 26.02.2024.
//

#include "OpenGLWidget.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "config.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  model_matrix_.setToIdentity();
  InitBuffers();
  InitShaderProgram();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.784f, 0.823f, 0.819f, 1.0f);
}
void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);
  if (is_data_load_) {
    glUseProgram(shader_program_);
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_TRUE,
                       model_matrix_.constData());
    glBindVertexArray(VAO);
    if (IsLines())
      glDrawElements(GL_LINES, (int)vertexes_->size(), GL_UNSIGNED_INT,
                     nullptr);
    if (IsPoints()) glDrawArrays(GL_POINTS, 0, (int)vertexes_->size());
    glBindVertexArray(0);
  }
}
std::optional<std::string> OpenGLWidget::GetShaderSource(
    const std::string &filename) {
  std::ifstream file(SHADER_PATH + filename);
  if (!file.is_open()) return std::nullopt;
  std::stringstream buf;
  buf << file.rdbuf();
  std::string fileData = buf.str();
  file.close();
  return fileData;
}

void OpenGLWidget::InitBuffers() {
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3,
                        (void *)nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
void OpenGLWidget::LoadDataToBuffers() {
  if (vertexes_ == nullptr || facets_ == nullptr) return;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               (int)(sizeof(GLfloat) * vertexes_->size()),
               vertexes_->data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (int)(sizeof(unsigned) * facets_->size()),
               facets_->data(), GL_STATIC_DRAW);
  is_data_load_ = true;
}
void OpenGLWidget::InitShaderProgram() {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::optional<std::string> stringShaderSourcesCode =
      GetShaderSource("point_vertex");
  if (!stringShaderSourcesCode) {
    std::cout << "SHADERS DON'T OPENED" << std::endl;
    return;
  }
  const char *charShaderSourceCode = stringShaderSourcesCode->c_str();
  glShaderSource(vertexShader, 1, &charShaderSourceCode, nullptr);
  glCompileShader(vertexShader);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  stringShaderSourcesCode = GetShaderSource("point_fragment");
  if (!stringShaderSourcesCode) {
    return;
  }
  charShaderSourceCode = stringShaderSourcesCode->c_str();
  glShaderSource(fragmentShader, 1, &charShaderSourceCode, nullptr);
  glCompileShader(fragmentShader);
  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertexShader);
  glAttachShader(shader_program_, fragmentShader);
  glLinkProgram(shader_program_);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mouse) {
  mouse_position_ = mouse->pos();
}

void OpenGLWidget::RotateCoordinateSystem(float angle, const QVector3D &axis) {
  model_matrix_.rotate(angle, axis);
  update();
}

void OpenGLWidget::InitModelMatrix() { model_matrix_.setToIdentity(); }

bool OpenGLWidget::IsLines() const { return vertexes_ != nullptr; }
bool OpenGLWidget::IsPoints() const { return facets_ != nullptr; }

void OpenGLWidget::SetVertexes(const std::vector<GLfloat> *vertexes) {
  vertexes_ = vertexes;
}
void OpenGLWidget::SetFacets(const std::vector<unsigned int> *facets) {
  facets_ = facets;
}
