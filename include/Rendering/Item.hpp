#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../GameData/BlockIds.hpp"
#include "../Gui/Rectangle.hpp"

class Item {
  private:
    GLuint m_vao;
    GLuint m_ebo;
    GLuint m_vbo;

  public:
    Item();
    Item(const Rectangle &area, const BlockId &block);
    ~Item();

    void render() const;
    void resize(const Rectangle &area, const BlockId &block) const;
};