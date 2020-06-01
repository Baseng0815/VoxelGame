#pragma once

#include <vector>

#include "Widget.h"
#include "../Rendering/RenderQuad.h"

class GUI;
class Text;
class Widget;

typedef std::vector<Widget*>::iterator WidgetIt;

enum StackMode {
    STACK_NONE, STACK_VERTICAL, STACK_HORIZONTAL
};

class Layout : public Widget {
    protected:
        std::vector<Widget*> m_widgets;

        // used for registering widgets
        GUI* m_gui;
        bool m_invertStack = false;
        StackMode m_stackMode = STACK_NONE;

        virtual void arrangeWidgets();

        void _draw(Shader& shader) const override;

    public:
        Layout(std::string id, GUI* gui);

        void setStackMode(StackMode stackMode, bool invertStack = false);
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