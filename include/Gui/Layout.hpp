#pragma once

#include <vector>

#include "Widget.hpp"
#include "../Rendering/RenderQuad.hpp"

class GUI;
class Text;
class Image;
class Widget;

typedef std::vector<Widget*>::iterator WidgetIt;

enum StackMode {
    STACK_NONE = 0x00, STACK_VERTICAL = 0x01, STACK_HORIZONTAL = 0x02,
};

// layouts can't have a MATCH_CONSTRAINT
// why? too lazy to implement
class Layout : public Widget {
    protected:
        std::vector<Widget*> m_widgets;

        StackMode m_stackMode = STACK_NONE;
        bool m_invertStack = false;
        bool m_invertStackWidgets = false;

        GUI &m_gui;

        virtual void arrangeWidgets();

        void _draw(const glm::mat4& projection) const override;
        void _updateArea() override;
        void _updateScreenElements() override;

    public:
        Layout(const std::string &id, GUI &gui);

        void setStackMode(StackMode stackMode, bool invertStack, bool invertStackWidgets = false);

        // create new widget
        template<class T>
        T* addWidget(const std::string& id);

        // add existing widget
        Widget* addWidget(Widget* widget);
        void removeWidget(const std::string& id);

        WidgetIt begin();
        WidgetIt end();
};
