#pragma once

#include "Widget.h"
#include <vector>

#include "RenderQuad.h"

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
        Layout(std::string id, GUI* gui, Layout* parent = nullptr);

        void setStackMode(StackMode stackMode, bool invertStack = false);
        void updateArea(Rectangle parent = Rectangle()) override;
        void updateScreenElements() override;

        // create new widget
        template<class T>
        T* addWidget(std::string id);

        // add existing widget
        Widget* addWidget(Widget* widget);
        void removeWidget(std::string id);
        bool isRootLayout() const;

        WidgetIt begin();
        WidgetIt end();
};
