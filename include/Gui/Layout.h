#pragma once

#include <vector>

#include "Widget.h"
#include "../Rendering/RenderQuad.h"

class GUI;
class Text;
class Image;
class Widget;

typedef std::vector<Widget*>::iterator WidgetIt;

enum StackMode {
    STACK_NONE = 0x00, STACK_VERTICAL = 0x01, STACK_HORIZONTAL = 0x02,
};

class Layout : public Widget {
    protected:
        std::vector<Widget*> m_widgets;

        // used for registering widgets
        GUI* m_gui;
        StackMode m_stackMode = STACK_NONE;
        bool m_invertStack = false;
        bool m_invertStackWidgets = false;

        virtual void arrangeWidgets();

        void _draw(const glm::mat4& projection) const override;

    public:
        Layout(std::string id, GUI* gui);

        void setStackMode(StackMode stackMode, bool invertStack, bool invertStackWidgets = false);
        void updateArea(const Rectangle& parent) override;
        void updateScreenElements() override;

        // create new widget
        template<class T>
        T* addWidget(std::string id);

        // add existing widget
        Widget* addWidget(Widget* widget);
        void removeWidget(std::string id);

        WidgetIt begin();
        WidgetIt end();
};
