#pragma once

#include <string>
#include <iostream>

#include <nanogui/nanogui.h>
#include <nanogui/opengl.h>

#include <cstdint>
#include <memory>
#include <utility>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace nanogui;

// NanoGUI example1

class GLTexture {
public:
    using handleType = std::unique_ptr<uint8_t[], void(*)(void*)>;
    GLTexture() = default;
    GLTexture(const std::string& textureName)
        : mTextureName(textureName), mTextureId(0) {}

    GLTexture(const std::string& textureName, GLint textureId)
        : mTextureName(textureName), mTextureId(textureId) {}

    GLTexture(const GLTexture& other) = delete;
    GLTexture(GLTexture&& other) noexcept
        : mTextureName(std::move(other.mTextureName)),
        mTextureId(other.mTextureId) {
        other.mTextureId = 0;
    }
    GLTexture& operator=(const GLTexture& other) = delete;
    GLTexture& operator=(GLTexture&& other) noexcept {
        mTextureName = std::move(other.mTextureName);
        std::swap(mTextureId, other.mTextureId);
        return *this;
    }
    ~GLTexture() noexcept {
        if (mTextureId)
            glDeleteTextures(1, &mTextureId);
    }

    GLuint texture() const { return mTextureId; }
    const std::string& textureName() const { return mTextureName; }

    /**
    *  Load a file in memory and create an OpenGL texture.
    *  Returns a handle type (an std::unique_ptr) to the loaded pixels.
    */
    handleType load(const std::string& fileName) {
        if (mTextureId) {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
        }
        int force_channels = 0;
        int w, h, n;
        handleType textureData(stbi_load(fileName.c_str(), &w, &h, &n, force_channels), stbi_image_free);
        if (!textureData)
            throw std::invalid_argument("Could not load texture data from file " + fileName);
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        GLint internalFormat;
        GLint format;
        switch (n) {
            case 1: internalFormat = GL_R8; format = GL_RED; break;
            case 2: internalFormat = GL_RG8; format = GL_RG; break;
            case 3: internalFormat = GL_RGB8; format = GL_RGB; break;
            case 4: internalFormat = GL_RGBA8; format = GL_RGBA; break;
            default: internalFormat = 0; format = 0; break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, textureData.get());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        return textureData;
    }

private:
    std::string mTextureName;
    GLuint mTextureId;
};

void main_window(){
    Screen *screen = new Screen(Eigen::Vector2i(800, 600), "RayTrace");

    FormHelper *gui = new FormHelper(screen);


    screen->setVisible(true);
    screen->performLayout();

}

void panel(FormHelper *gui){
        ref<Window> panel = gui->addWindow(Vector2i(10, 10), "Panel");
}

void image_show(FormHelper *gui){
    ref<Window> image_show = gui->addWindow(Vector2i(500, 300), "Image");
    image_show->setPosition(Vector2i(4, 4));
    GLTexture texture("rendered");
    auto img_data = texture.load("D:/rayTraceCpp/7ed17ffcb9c5ba5b3c6a7f197295f86d_0.png");
    auto imageView = new ImageView(image_show, texture.texture());
    auto texture_pair = std::make_pair(std::move(texture), std::move(img_data));

    imageView->setGridThreshold(20);
        imageView->setPixelInfoThreshold(20);
        imageView->setPixelInfoCallback(
            [imageView, texture_pair](const Vector2i& index) -> std::pair<std::string, Color> {
            auto& imageData = texture_pair.second;
            auto& textureSize = imageView->imageSize();
            std::string stringData{};
            uint16_t channelSum = 0;
            for (int i = 0; i != 4; ++i) {
                auto& channelData = imageData[4*index.y()*textureSize.x() + 4*index.x() + i];
                channelSum += channelData;
                stringData += (std::to_string(static_cast<int>(channelData)) + "\n");
            }
            float intensity = static_cast<float>(255 - (channelSum / 4)) / 255.0f;
            float colorScale = intensity > 0.5f ? (intensity + 1) / 2 : intensity / 2;
            Color textColor = Color(colorScale, 1.0f);
            return { stringData, textColor };
        });

}