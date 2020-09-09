#include "ui.h"

UI::UI() : Billboard()
{
    mUiElement = true;
    mVisible = false;
}

UI::UI(const char *texture) : Billboard(texture)
{
    mUiElement = true;
    mVisible = false;

}
