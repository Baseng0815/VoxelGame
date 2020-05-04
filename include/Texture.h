#include <GL/glew.h>

#include "Resource.h"

#include <experimental/string_view>

using namespace std::experimental;

class Texture : public Resource {
    private:
        GLuint m_texture;
        int m_width, m_height, m_channels;

    public:
        Texture(string_view path);
        ~Texture();

        void bindTexture(int textureUnit);
        int getWidth() const;
        int getHeight() const;
        int getTexture() const;
};
