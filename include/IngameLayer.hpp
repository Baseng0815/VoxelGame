#pragma once

#include "GameLayer.hpp"

#include <atomic>
#include <entt/entt.hpp>
#include <vector>

#include "Gui/IngameLayerGUI.hpp"

#include "Events/CallbackHandle.hpp"
#include "Resources/TextureAtlas.hpp"

struct ScrollEvent;

class IngameLayer : public GameLayer {
  private:
    std::vector<std::unique_ptr<System>> m_systems;
    Registry_T m_registry;
    TextureAtlas m_atlas;

    CallbackHandle<const KeyEvent&> m_keyEventHandle;
    void handleKeys(const KeyEvent&);

    int m_time = 0;
    int m_frameCounter = 0;

    IngameLayerGUI m_gui;

  public:
    IngameLayer(Application* application);
    ~IngameLayer();

    void update(int dt) override;
};
