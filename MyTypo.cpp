#include "MyTypo.hpp"

MyTypo::MyTypo (int style, int foreground, int background)
{
    this->foreground = foreground;
    this->background = background;
    this->style      = style;
    this->opened     = false;
}

void MyTypo::set_foreground (int color) { foreground = color; }
void MyTypo::set_background (int color) { background = color; }
void MyTypo::set_style      (int color) { style = color; }

int MyTypo::get_foreground (void) { return foreground; }
int MyTypo::get_background (void) { return background; }
int MyTypo::get_style      (void) { return style; }
bool MyTypo::get_opened    (void) { return opened; }

void MyTypo::toogleOpened (void)
{
    this->opened = !this->opened;
}

void MyTypo::setParameters (int style, int foreground, int background)
{
    this->foreground = foreground;
    this->background = background;
    this->style      = style;
}

std::ostream& MyTypo::openTag (std::ostream& ost)
{
    if (this->background == B_TRANSPARENT)
        return ost << "\e[" << style << ";" << foreground << "m";
    else
        return ost << "\e[" << style << ";" << foreground << ";" << background << "m";
}

std::ostream& MyTypo::closeTag (std::ostream& ost)
{
    return ost << "\e[0m";
}

std::ostream& operator<< (std::ostream& ost, MyTypo& mt)
{
    mt.toogleOpened();
    bool opened = mt.get_opened();
    if (opened)
        return mt.openTag(ost);
    else
        return mt.closeTag(ost);
}
