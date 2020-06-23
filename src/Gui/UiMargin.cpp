#include "../../include/Gui/UiMargin.h"

void Margin::vertical(float value) {
    top = bottom = value;
}

void Margin::horizontal(float value) {
    left = right = value;
}

void Margin::both(float value) {
    top = bottom = left = right = value;
}
