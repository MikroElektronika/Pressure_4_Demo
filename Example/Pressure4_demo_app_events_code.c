#include "Pressure4_demo_app_objects.h"
#include "Pressure4_demo_app_resources.h"

//--------------------- User code ---------------------//

extern _button;
extern _flag;
extern pressButton;

//----------------- End of User code ------------------//


void Button1OnClick()
{
    _button = 0;
    _flag += 1;
    pressButton = 1;
    
}
void Button2OnClick() 
{
    _button = 1;
    _flag += 1;
    pressButton = 1;
}
void Button3OnClick() 
{
    _button = 2;
    _flag += 1;
    pressButton = 1;
}
void Button4OnClick() 
{
    _button = 3;
    _flag += 1;
    pressButton = 1;
}
void Button5OnClick() 
{
    _button = 4;
    _flag += 1;
    pressButton = 1;
}
void Button6OnClick() 
{
    _button = 5;
    _flag += 1;
    pressButton = 1;
}
void Button7OnClick() 
{
    _button = 6;
    _flag += 1;
    pressButton = 1;
}
void Button8OnClick() 
{
    _button = 7;
    _flag += 1;
    pressButton = 1;
}
void Button9OnClick() 
{
    _button = 8;
    _flag += 1;
    pressButton = 1;
}
void Button10OnClick() 
{
    _button = 9;
    _flag += 1;
    pressButton = 1;
}