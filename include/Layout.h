#pragma once

#include "Widget.h"
#include <vector>

#include "RenderQuad.h"

class GUI;
class Widget;

typedef std::vector<Widget*>::iterator WidgetIt;

class Layout : public Widget {
    protected:
        std::vector<Widget*> m_widgets;

        // used for registering widgets
        GUI* m_gui;

        virtual void arrangeWidgets();

        void _draw(Shader& shader) const override;

    public:
        Layout(std::string id, GUI* gui, Layout* parent = nullptr);

        void resize() override;

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
