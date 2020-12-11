#pragma once

// TODO add relative margin/padding
// margin ^= outside of widget
struct Margin {
    float left = 0, right = 0, top = 0, bottom = 0;

    // apply padding equally in x, y and x/y direction
    void vertical(float value);
    void horizontal(float value);
    void both(float value);
};

// padding ^= inside of widget
using Padding = Margin;
