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
    TEXTURE_GUI_HOTBAR,
    TEXTURE_GUI_HOTBAR_SELECTOR,
    TEXTURE_GUI_INVENTORY,
    TEXTURE_GUI_SLOT_SELECTOR,

    SHADER_MIN,
    SHADER_MESH_RENDER_TEXTURE,
    SHADER_MESH_RENDER_COLOR,
    SHADER_TEXTURE_QUAD,
    SHADER_COLOR_QUAD,
    SHADER_WATER,
    SHADER_SKYBOX,
    SHADER_TEXT,
    SHADER_MVP_COLOR,
    SHADER_MAX,
    SHADER_FRAMEBUFFER,
    SHADER_BLOCK_SELECTOR,

    MATERIAL_CHUNK_BLOCKS_CULLED,
    MATERIAL_CHUNK_BLOCKS_NON_CULLED,
    MATERIAL_CLOUDS,
    MATERIAL_GRASS,
    MATERIAL_WATER,

    FONT_KORURI
};
