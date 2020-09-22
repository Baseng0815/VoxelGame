#pragma once

#include "UiMargin.hpp"
#include "UiConstraints.hpp"

#include "../Color.hpp"

struct UiProperties {
    bool isVisible = true;
    bool receiveEvents = true;
    UiConstraints constraints;
    Margin margin;
    Padding padding;

    Color backgroundColor = Color::Transparent;
    Color foregroundColor = Color::White;
    Color hoverBackground = Color::Transparent;
    Color hoverForeground = Color::White;
    Color pressBackground = Color::Transparent;
    Color pressForeground = Color::White;
};
