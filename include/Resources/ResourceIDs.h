#pragma once

// give all resources an ID

using ResourceID = unsigned long;

enum RESOURCES {
    TEXTURE_ATLAS,
    TEXTURE_BACKGROUND_MAIN_MENU,
    TEXTURE_WHITE,
    TEXTURE_BLACK,
    TEXTURE_CROSSHAIR,
    TEXTURE_CUBE_SKYBOX,

    SHADER_MESH_RENDER_TEXTURE,
    SHADER_MESH_RENDER_COLOR,
    SHADER_TEXTURE_QUAD,
    SHADER_COLOR_QUAD,
    SHADER_SKYBOX,
    SHADER_TEXT,
    SHADER_MVP_COLOR,

    MATERIAL_CHUNK_BLOCKS,
    MATERIAL_CLOUDS,

    FONT_KORURI
};
