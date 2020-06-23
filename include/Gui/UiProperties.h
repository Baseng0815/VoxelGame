#pragma once

#include "UiMargin.h"
#include "UiConstraints.h"

#include "../Color.h"

// TODO add content-align (e.g. for text inside of large widget)
struct UiProperties {
    bool isVisible;
    UiConstraints constraints;
    Margin margin;
    Padding padding;

    Color backgroundColor = Color::Black;
    Color foregroundColor = Color::White;
    Color hoverBackground = Color::Black;
    Color hoverForeground = Color::White;
    Color pressBackground = Color::Black;
    Color pressForeground = Color::White;
};
