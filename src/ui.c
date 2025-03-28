#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <ui.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#define MAX_CHARAS 128

typedef struct
{
    GLuint textureID;       // OpenGL texture ID
    int width, height;      // Bitmap size
    int bearingX, bearingY; // Offset from baseline
    int advance;            // Horizontal advance
} Character;

Character characters[MAX_CHARAS]; // Store ASCII characters

void loadFont(const char *fontPath)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("Failed to initialize FreeType\n");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
    {
        printf("Failed to load font: %s\n", fontPath);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); // Set font size

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("Failed to load glyph: %c\n", c);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        characters[c] = (Character){
            texture,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            face->glyph->bitmap_left, face->glyph->bitmap_top,
            face->glyph->advance.x >> 6 // Convert from 1/64th pixels to pixels
        };
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void renderTextPlaceholder(float x, float y, const char *text)
{
    printf("Render text at (%.2f, %.2f): %s\n", x, y, text);
}