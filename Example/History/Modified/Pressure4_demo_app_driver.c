#include "Pressure4_demo_app_objects.h"
#include "Pressure4_demo_app_resources.h"
#include "built_in.h"


// TFT module connections
sbit TFT_BLED at GPIOF_ODR.B10;
sbit TFT_CS at GPIOF_ODR.B13;
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RD at GPIOF_ODR.B12;
sbit TFT_RS at GPIOF_ODR.B15;
sbit TFT_RST at GPIOF_ODR.B14;
sbit TFT_WR at GPIOF_ODR.B11;
// End TFT module connections


// Global variables
unsigned int Xcoord, Ycoord;
char PenDown;
void *PressedObject;
int PressedObjectType;
unsigned int caption_length, caption_height;
unsigned int display_width, display_height;

int _object_count;
unsigned short object_pressed;
TButton *local_button;
TButton *exec_button;
int button_order;
TLabel *local_label;
TLabel *exec_label;
int label_order;
TBox *local_box;
TBox *exec_box;
int box_order;


void Write_to_Data_Lines(unsigned char _hi, unsigned char _lo) {
  unsigned int temp;
  temp = GPIOE_ODR;
  temp &= 0x00FF;
  GPIOE_ODR = temp | (_hi << 8);
  temp = GPIOG_ODR;
  temp &= 0xFF00;
  GPIOG_ODR = temp | _lo;
}

void Set_Index(unsigned short index) {
  TFT_RS = 0;
  Write_to_Data_Lines(0, index);
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

void Write_Command(unsigned short cmd) {
  TFT_RS = 1;
  Write_to_Data_Lines(0, cmd);
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

void Write_Data(unsigned int _data) {
  TFT_RS = 1;
  Write_to_Data_Lines(Hi(_data), Lo(_data));
  TFT_WR = 0;
  asm nop;
  TFT_WR = 1;
}

static void InitializeTouchPanel() {
  TFT_Set_Active(Set_Index, Write_Command, Write_Data);
  TFT_Init_SSD1963(480, 272);

  TFT_Set_DBC_SSD1963(255);

  PenDown = 0;
  PressedObject = 0;
  PressedObjectType = -1;
}

void Calibrate() {
  TFT_Fill_Screen(CL_BLACK);
  TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
  TFT_Write_Text("Touch points on the screen as they", 125, 121);
  TFT_Write_Text("appear to calibrate touchscreen.", 135, 151);
  STMPE610_ClearInterruptFlagsAndFIFO();
  TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
  TFT_Circle(0, 0, 3);
  STMPE610_CalibratePoint(STMPE610_FIRST_CORNER);
  Delay_ms(500);
  TFT_Set_Brush(1, CL_BLACK, 0, 0, 0, 0);
  TFT_Circle(0, 0, 3);
  STMPE610_ClearInterruptFlagsAndFIFO();
  TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
  TFT_Circle(0, TFT_DISP_HEIGHT-1, 3);
  STMPE610_CalibratePoint(STMPE610_SECOND_CORNER);
  Delay_ms(500);
  TFT_Set_Brush(1, CL_BLACK, 0, 0, 0, 0);
  TFT_Circle(0, TFT_DISP_HEIGHT-1, 3);
  STMPE610_ClearInterruptFlagsAndFIFO();
  TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
  TFT_Circle(TFT_DISP_WIDTH-1, TFT_DISP_HEIGHT-1, 3);
  STMPE610_CalibratePoint(STMPE610_THIRD_CORNER);
  Delay_ms(500);
  TFT_Set_Brush(1, CL_BLACK, 0, 0, 0, 0);
  TFT_Circle(TFT_DISP_WIDTH-1, TFT_DISP_HEIGHT-1, 3);
  STMPE610_ClearInterruptFlagsAndFIFO();
  TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
  TFT_Circle(TFT_DISP_WIDTH-1, 0, 3);
  STMPE610_CalibratePoint(STMPE610_FOURTH_CORNER);
  TFT_Set_Brush(1, CL_BLACK, 0, 0, 0, 0);
  TFT_Circle(TFT_DISP_WIDTH-1, 0, 3);
  Delay_ms(500);
}


/////////////////////////
  TScreen*  CurrentScreen;

  TScreen                Screen1;
  TLine                  Line1;
  TLine                  Line2;
  TLine                  Line3;
  TLine                  Line4;
  TBox                   Box1;
  TBox                   Box2;
  TLabel                 Temp_value;
char Temp_value_Caption[5] = "0.00";

  TLabel                 Pressure_value;
char Pressure_value_Caption[5] = "0.00";

  TLabel                 Label1;
char Label1_Caption[5] = "TIME";

  TLabel                 Label2;
char Label2_Caption[4] = "[C]";

  TBox                   Box3;
  TLabel                 Label13;
char Label13_Caption[9] = "Pressure";

  TBox                   Box4;
  TLabel                 Label14;
char Label14_Caption[12] = "Temperature";

  TLine                  Line7;
  TLine                  Line8;
  TLine                  Line9;
  TLine                  Line10;
  TLabel                 Label15;
char Label15_Caption[5] = "TIME";

  TLabel                 Label3;
char Label3_Caption[6] = "[hPa]";

  TLabel                 * const code Screen1_Labels[8]=
         {
         &Temp_value,          
         &Pressure_value,      
         &Label1,              
         &Label2,              
         &Label13,             
         &Label14,             
         &Label15,             
         &Label3               
         };
  TBox                   * const code Screen1_Boxes[4]=
         {
         &Box1,                
         &Box2,                
         &Box3,                
         &Box4                 
         };
  TLine                  * const code Screen1_Lines[8]=
         {
         &Line1,               
         &Line2,               
         &Line3,               
         &Line4,               
         &Line7,               
         &Line8,               
         &Line9,               
         &Line10               
         };

  TScreen                Screen2;
  TLabel                 Label16;
char Label16_Caption[26] = "Set today\'s date and time";

  TButton               Button1;
char Button1_Caption[2] = "0";

  TButton               Button2;
char Button2_Caption[2] = "1";

  TButton               Button3;
char Button3_Caption[2] = "2";

  TButton               Button4;
char Button4_Caption[2] = "3";

  TButton               Button5;
char Button5_Caption[2] = "4";

  TButton               Button6;
char Button6_Caption[2] = "5";

  TButton               Button7;
char Button7_Caption[2] = "6";

  TButton               Button8;
char Button8_Caption[2] = "7";

  TButton               Button9;
char Button9_Caption[2] = "8";

  TButton               Button10;
char Button10_Caption[2] = "9";

  TBox                   Box5;
  TLabel                 hour1;
char hour1_Caption[2] = "0";

  TBox                   Box6;
  TLabel                 minute1;
char minute1_Caption[2] = "0";

  TBox                   Box7;
  TLabel                 sec1;
char sec1_Caption[2] = "0";

  TLabel                 Label22;
char Label22_Caption[2] = ":";

  TLabel                 Label23;
char Label23_Caption[2] = ":";

  TBox                   Box8;
  TLabel                 date1;
char date1_Caption[2] = "0";

  TBox                   Box9;
  TLabel                 month1;
char month1_Caption[2] = "0";

  TLabel                 Label30;
char Label30_Caption[2] = ".";

  TLabel                 Label31;
char Label31_Caption[2] = ".";

  TBox                   Box11;
  TLabel                 year1;
char year1_Caption[2] = "0";

  TLabel                 Label17;
char Label17_Caption[2] = "0";

  TLabel                 Label18;
char Label18_Caption[2] = "0";

  TLabel                 Label19;
char Label19_Caption[2] = "0";

  TLabel                 Label20;
char Label20_Caption[2] = "0";

  TLabel                 Label21;
char Label21_Caption[2] = "0";

  TLabel                 Label24;
char Label24_Caption[2] = "0";

  TLabel                 Label25;
char Label25_Caption[8] = "Time - ";

  TLabel                 Label26;
char Label26_Caption[8] = "Date - ";

  TBox                   Box10;
  TLabel                 Label4;
char Label4_Caption[9] = "Alarm[s]";

  TLabel                 Alarm0;
char Alarm0_Caption[2] = "0";

  TLabel                 Alarm1;
char Alarm1_Caption[2] = "0";

  TLabel                 Alarm2;
char Alarm2_Caption[2] = "0";

  TLabel                 Alarm3;
char Alarm3_Caption[2] = "0";

  TButton                * const code Screen2_Buttons[10]=
         {
         &Button1,             
         &Button2,             
         &Button3,             
         &Button4,             
         &Button5,             
         &Button6,             
         &Button7,             
         &Button8,             
         &Button9,             
         &Button10             
         };
  TLabel                 * const code Screen2_Labels[24]=
         {
         &Label16,             
         &hour1,               
         &minute1,             
         &sec1,                
         &Label22,             
         &Label23,             
         &date1,               
         &month1,              
         &Label30,             
         &Label31,             
         &year1,               
         &Label17,             
         &Label18,             
         &Label19,             
         &Label20,             
         &Label21,             
         &Label24,             
         &Label25,             
         &Label26,             
         &Label4,              
         &Alarm0,              
         &Alarm1,              
         &Alarm2,              
         &Alarm3               
         };
  TBox                   * const code Screen2_Boxes[7]=
         {
         &Box5,                
         &Box6,                
         &Box7,                
         &Box8,                
         &Box9,                
         &Box11,               
         &Box10                
         };



static void InitializeObjects() {
  Screen1.Color                     = 0xFFFF;
  Screen1.Width                     = 480;
  Screen1.Height                    = 272;
  Screen1.ButtonsCount              = 0;
  Screen1.LabelsCount               = 8;
  Screen1.Labels                    = Screen1_Labels;
  Screen1.BoxesCount                = 4;
  Screen1.Boxes                     = Screen1_Boxes;
  Screen1.LinesCount                = 8;
  Screen1.Lines                     = Screen1_Lines;
  Screen1.ObjectsCount              = 20;

  Screen2.Color                     = 0xFFFF;
  Screen2.Width                     = 480;
  Screen2.Height                    = 272;
  Screen2.ButtonsCount              = 10;
  Screen2.Buttons                   = Screen2_Buttons;
  Screen2.LabelsCount               = 24;
  Screen2.Labels                    = Screen2_Labels;
  Screen2.BoxesCount                = 7;
  Screen2.Boxes                     = Screen2_Boxes;
  Screen2.LinesCount                = 0;
  Screen2.ObjectsCount              = 41;


  Line1.OwnerScreen     = &Screen1;
  Line1.Order           = 0;
  Line1.First_Point_X   = 40;
  Line1.First_Point_Y   = 24;
  Line1.Second_Point_X  = 40;
  Line1.Second_Point_Y  = 128;
  Line1.Visible         = 1;
  Line1.Pen_Width       = 2;
  Line1.Color           = 0xF800;

  Line2.OwnerScreen     = &Screen1;
  Line2.Order           = 1;
  Line2.First_Point_X   = 39;
  Line2.First_Point_Y   = 24;
  Line2.Second_Point_X  = 47;
  Line2.Second_Point_Y  = 34;
  Line2.Visible         = 1;
  Line2.Pen_Width       = 2;
  Line2.Color           = 0xF800;

  Line3.OwnerScreen     = &Screen1;
  Line3.Order           = 2;
  Line3.First_Point_X   = 39;
  Line3.First_Point_Y   = 25;
  Line3.Second_Point_X  = 33;
  Line3.Second_Point_Y  = 34;
  Line3.Visible         = 1;
  Line3.Pen_Width       = 2;
  Line3.Color           = 0xF800;

  Line4.OwnerScreen     = &Screen1;
  Line4.Order           = 3;
  Line4.First_Point_X   = 290;
  Line4.First_Point_Y   = 120;
  Line4.Second_Point_X  = 25;
  Line4.Second_Point_Y  = 120;
  Line4.Visible         = 1;
  Line4.Pen_Width       = 2;
  Line4.Color           = 0x632C;

  Box1.OwnerScreen     = &Screen1;
  Box1.Order           = 4;
  Box1.Left            = 350;
  Box1.Top             = 26;
  Box1.Width           = 120;
  Box1.Height          = 38;
  Box1.Pen_Width       = 2;
  Box1.Pen_Color       = 0x3660;
  Box1.Visible         = 1;
  Box1.Active          = 1;
  Box1.Transparent     = 1;
  Box1.Gradient        = 1;
  Box1.Gradient_Orientation = 0;
  Box1.Gradient_Start_Color = 0xFFFF;
  Box1.Gradient_End_Color = 0xFFFF;
  Box1.Color           = 0xFFFF;
  Box1.PressColEnabled = 1;
  Box1.Press_Color     = 0xE71C;
  Box1.OnUpPtr         = 0;
  Box1.OnDownPtr       = 0;
  Box1.OnClickPtr      = 0;
  Box1.OnPressPtr      = 0;

  Box2.OwnerScreen     = &Screen1;
  Box2.Order           = 5;
  Box2.Left            = 350;
  Box2.Top             = 91;
  Box2.Width           = 120;
  Box2.Height          = 38;
  Box2.Pen_Width       = 2;
  Box2.Pen_Color       = 0x633F;
  Box2.Visible         = 1;
  Box2.Active          = 1;
  Box2.Transparent     = 1;
  Box2.Gradient        = 1;
  Box2.Gradient_Orientation = 0;
  Box2.Gradient_Start_Color = 0xFFFF;
  Box2.Gradient_End_Color = 0xFFFF;
  Box2.Color           = 0xFFFF;
  Box2.PressColEnabled = 1;
  Box2.Press_Color     = 0xE71C;
  Box2.OnUpPtr         = 0;
  Box2.OnDownPtr       = 0;
  Box2.OnClickPtr      = 0;
  Box2.OnPressPtr      = 0;

  Temp_value.OwnerScreen     = &Screen1;
  Temp_value.Order           = 6;
  Temp_value.Left            = 355;
  Temp_value.Top             = 35;
  Temp_value.Width           = 30;
  Temp_value.Height          = 17;
  Temp_value.Visible         = 1;
  Temp_value.Active          = 1;
  Temp_value.Caption         = Temp_value_Caption;
  Temp_value.FontName        = Tahoma12x16_Regular;
  Temp_value.Font_Color      = 0x0000;
  Temp_value.VerticalText    = 0;
  Temp_value.OnUpPtr         = 0;
  Temp_value.OnDownPtr       = 0;
  Temp_value.OnClickPtr      = 0;
  Temp_value.OnPressPtr      = 0;

  Pressure_value.OwnerScreen     = &Screen1;
  Pressure_value.Order           = 7;
  Pressure_value.Left            = 354;
  Pressure_value.Top             = 100;
  Pressure_value.Width           = 30;
  Pressure_value.Height          = 17;
  Pressure_value.Visible         = 1;
  Pressure_value.Active          = 1;
  Pressure_value.Caption         = Pressure_value_Caption;
  Pressure_value.FontName        = Tahoma12x16_Regular;
  Pressure_value.Font_Color      = 0x0000;
  Pressure_value.VerticalText    = 0;
  Pressure_value.OnUpPtr         = 0;
  Pressure_value.OnDownPtr       = 0;
  Pressure_value.OnClickPtr      = 0;
  Pressure_value.OnPressPtr      = 0;

  Label1.OwnerScreen     = &Screen1;
  Label1.Order           = 8;
  Label1.Left            = 289;
  Label1.Top             = 237;
  Label1.Width           = 33;
  Label1.Height          = 15;
  Label1.Visible         = 1;
  Label1.Active          = 1;
  Label1.Caption         = Label1_Caption;
  Label1.FontName        = Tahoma13x13_Bold;
  Label1.Font_Color      = 0x632C;
  Label1.VerticalText    = 0;
  Label1.OnUpPtr         = 0;
  Label1.OnDownPtr       = 0;
  Label1.OnClickPtr      = 0;
  Label1.OnPressPtr      = 0;

  Label2.OwnerScreen     = &Screen1;
  Label2.Order           = 9;
  Label2.Left            = 7;
  Label2.Top             = 187;
  Label2.Width           = 22;
  Label2.Height          = 15;
  Label2.Visible         = 1;
  Label2.Active          = 1;
  Label2.Caption         = Label2_Caption;
  Label2.FontName        = Tahoma13x13_Bold;
  Label2.Font_Color      = 0xF800;
  Label2.VerticalText    = 0;
  Label2.OnUpPtr         = 0;
  Label2.OnDownPtr       = 0;
  Label2.OnClickPtr      = 0;
  Label2.OnPressPtr      = 0;

  Box3.OwnerScreen     = &Screen1;
  Box3.Order           = 10;
  Box3.Left            = 350;
  Box3.Top             = 13;
  Box3.Width           = 37;
  Box3.Height          = 8;
  Box3.Pen_Width       = 1;
  Box3.Pen_Color       = 0x3660;
  Box3.Visible         = 1;
  Box3.Active          = 1;
  Box3.Transparent     = 1;
  Box3.Gradient        = 1;
  Box3.Gradient_Orientation = 0;
  Box3.Gradient_Start_Color = 0x3660;
  Box3.Gradient_End_Color = 0x3660;
  Box3.Color           = 0x3660;
  Box3.PressColEnabled = 1;
  Box3.Press_Color     = 0xE71C;
  Box3.OnUpPtr         = 0;
  Box3.OnDownPtr       = 0;
  Box3.OnClickPtr      = 0;
  Box3.OnPressPtr      = 0;

  Label13.OwnerScreen     = &Screen1;
  Label13.Order           = 11;
  Label13.Left            = 390;
  Label13.Top             = 8;
  Label13.Width           = 53;
  Label13.Height          = 15;
  Label13.Visible         = 1;
  Label13.Active          = 1;
  Label13.Caption         = Label13_Caption;
  Label13.FontName        = Tahoma13x13_Bold;
  Label13.Font_Color      = 0x0000;
  Label13.VerticalText    = 0;
  Label13.OnUpPtr         = 0;
  Label13.OnDownPtr       = 0;
  Label13.OnClickPtr      = 0;
  Label13.OnPressPtr      = 0;

  Box4.OwnerScreen     = &Screen1;
  Box4.Order           = 12;
  Box4.Left            = 350;
  Box4.Top             = 75;
  Box4.Width           = 37;
  Box4.Height          = 8;
  Box4.Pen_Width       = 1;
  Box4.Pen_Color       = 0x633F;
  Box4.Visible         = 1;
  Box4.Active          = 1;
  Box4.Transparent     = 1;
  Box4.Gradient        = 1;
  Box4.Gradient_Orientation = 0;
  Box4.Gradient_Start_Color = 0x633F;
  Box4.Gradient_End_Color = 0x633F;
  Box4.Color           = 0x633F;
  Box4.PressColEnabled = 1;
  Box4.Press_Color     = 0xE71C;
  Box4.OnUpPtr         = 0;
  Box4.OnDownPtr       = 0;
  Box4.OnClickPtr      = 0;
  Box4.OnPressPtr      = 0;

  Label14.OwnerScreen     = &Screen1;
  Label14.Order           = 13;
  Label14.Left            = 390;
  Label14.Top             = 71;
  Label14.Width           = 77;
  Label14.Height          = 15;
  Label14.Visible         = 1;
  Label14.Active          = 1;
  Label14.Caption         = Label14_Caption;
  Label14.FontName        = Tahoma13x13_Bold;
  Label14.Font_Color      = 0x0000;
  Label14.VerticalText    = 0;
  Label14.OnUpPtr         = 0;
  Label14.OnDownPtr       = 0;
  Label14.OnClickPtr      = 0;
  Label14.OnPressPtr      = 0;

  Line7.OwnerScreen     = &Screen1;
  Line7.Order           = 14;
  Line7.First_Point_X   = 40;
  Line7.First_Point_Y   = 151;
  Line7.Second_Point_X  = 40;
  Line7.Second_Point_Y  = 255;
  Line7.Visible         = 1;
  Line7.Pen_Width       = 2;
  Line7.Color           = 0xF800;

  Line8.OwnerScreen     = &Screen1;
  Line8.Order           = 15;
  Line8.First_Point_X   = 40;
  Line8.First_Point_Y   = 152;
  Line8.Second_Point_X  = 47;
  Line8.Second_Point_Y  = 161;
  Line8.Visible         = 1;
  Line8.Pen_Width       = 2;
  Line8.Color           = 0xF800;

  Line9.OwnerScreen     = &Screen1;
  Line9.Order           = 16;
  Line9.First_Point_X   = 40;
  Line9.First_Point_Y   = 151;
  Line9.Second_Point_X  = 33;
  Line9.Second_Point_Y  = 160;
  Line9.Visible         = 1;
  Line9.Pen_Width       = 2;
  Line9.Color           = 0xF800;

  Line10.OwnerScreen     = &Screen1;
  Line10.Order           = 17;
  Line10.First_Point_X   = 288;
  Line10.First_Point_Y   = 245;
  Line10.Second_Point_X  = 24;
  Line10.Second_Point_Y  = 245;
  Line10.Visible         = 1;
  Line10.Pen_Width       = 2;
  Line10.Color           = 0x632C;

  Label15.OwnerScreen     = &Screen1;
  Label15.Order           = 18;
  Label15.Left            = 290;
  Label15.Top             = 112;
  Label15.Width           = 33;
  Label15.Height          = 15;
  Label15.Visible         = 1;
  Label15.Active          = 1;
  Label15.Caption         = Label15_Caption;
  Label15.FontName        = Tahoma13x13_Bold;
  Label15.Font_Color      = 0x632C;
  Label15.VerticalText    = 0;
  Label15.OnUpPtr         = 0;
  Label15.OnDownPtr       = 0;
  Label15.OnClickPtr      = 0;
  Label15.OnPressPtr      = 0;

  Label3.OwnerScreen     = &Screen1;
  Label3.Order           = 19;
  Label3.Left            = 2;
  Label3.Top             = 53;
  Label3.Width           = 35;
  Label3.Height          = 15;
  Label3.Visible         = 1;
  Label3.Active          = 1;
  Label3.Caption         = Label3_Caption;
  Label3.FontName        = Tahoma13x13_Bold;
  Label3.Font_Color      = 0xF800;
  Label3.VerticalText    = 0;
  Label3.OnUpPtr         = 0;
  Label3.OnDownPtr       = 0;
  Label3.OnClickPtr      = 0;
  Label3.OnPressPtr      = 0;

  Label16.OwnerScreen     = &Screen2;
  Label16.Order           = 0;
  Label16.Left            = 7;
  Label16.Top             = 10;
  Label16.Width           = 331;
  Label16.Height          = 39;
  Label16.Visible         = 1;
  Label16.Active          = 0;
  Label16.Caption         = Label16_Caption;
  Label16.FontName        = Segoe_UI_Semibold26x37_Bold;
  Label16.Font_Color      = 0x0000;
  Label16.VerticalText    = 0;
  Label16.OnUpPtr         = 0;
  Label16.OnDownPtr       = 0;
  Label16.OnClickPtr      = 0;
  Label16.OnPressPtr      = 0;

  Button1.OwnerScreen     = &Screen2;
  Button1.Order           = 1;
  Button1.Left            = 40;
  Button1.Top             = 152;
  Button1.Width           = 70;
  Button1.Height          = 50;
  Button1.Pen_Width       = 1;
  Button1.Pen_Color       = 0xFFFF;
  Button1.Visible         = 1;
  Button1.Active          = 1;
  Button1.Transparent     = 1;
  Button1.Caption         = Button1_Caption;
  Button1.TextAlign       = _taCenter;
  Button1.TextAlignVertical= _tavMiddle;
  Button1.FontName        = Tahoma11x13_Regular;
  Button1.PressColEnabled = 1;
  Button1.Font_Color      = 0xFFFF;
  Button1.VerticalText    = 0;
  Button1.Gradient        = 1;
  Button1.Gradient_Orientation = 0;
  Button1.Gradient_Start_Color = 0x0000;
  Button1.Gradient_End_Color = 0x0000;
  Button1.Color           = 0x0000;
  Button1.Press_Color     = 0xE71C;
  Button1.OnUpPtr         = 0;
  Button1.OnDownPtr       = 0;
  Button1.OnClickPtr      = Button1OnClick;
  Button1.OnPressPtr      = 0;

  Button2.OwnerScreen     = &Screen2;
  Button2.Order           = 2;
  Button2.Left            = 120;
  Button2.Top             = 152;
  Button2.Width           = 70;
  Button2.Height          = 50;
  Button2.Pen_Width       = 1;
  Button2.Pen_Color       = 0xFFFF;
  Button2.Visible         = 1;
  Button2.Active          = 1;
  Button2.Transparent     = 1;
  Button2.Caption         = Button2_Caption;
  Button2.TextAlign       = _taCenter;
  Button2.TextAlignVertical= _tavMiddle;
  Button2.FontName        = Tahoma11x13_Regular;
  Button2.PressColEnabled = 1;
  Button2.Font_Color      = 0xFFFF;
  Button2.VerticalText    = 0;
  Button2.Gradient        = 1;
  Button2.Gradient_Orientation = 0;
  Button2.Gradient_Start_Color = 0x0000;
  Button2.Gradient_End_Color = 0x0000;
  Button2.Color           = 0x0000;
  Button2.Press_Color     = 0xE71C;
  Button2.OnUpPtr         = 0;
  Button2.OnDownPtr       = 0;
  Button2.OnClickPtr      = Button2OnClick;
  Button2.OnPressPtr      = 0;

  Button3.OwnerScreen     = &Screen2;
  Button3.Order           = 3;
  Button3.Left            = 200;
  Button3.Top             = 152;
  Button3.Width           = 70;
  Button3.Height          = 50;
  Button3.Pen_Width       = 1;
  Button3.Pen_Color       = 0xFFFF;
  Button3.Visible         = 1;
  Button3.Active          = 1;
  Button3.Transparent     = 1;
  Button3.Caption         = Button3_Caption;
  Button3.TextAlign       = _taCenter;
  Button3.TextAlignVertical= _tavMiddle;
  Button3.FontName        = Tahoma11x13_Regular;
  Button3.PressColEnabled = 1;
  Button3.Font_Color      = 0xFFFF;
  Button3.VerticalText    = 0;
  Button3.Gradient        = 1;
  Button3.Gradient_Orientation = 0;
  Button3.Gradient_Start_Color = 0x0000;
  Button3.Gradient_End_Color = 0x0000;
  Button3.Color           = 0x0000;
  Button3.Press_Color     = 0xE71C;
  Button3.OnUpPtr         = 0;
  Button3.OnDownPtr       = 0;
  Button3.OnClickPtr      = Button3OnClick;
  Button3.OnPressPtr      = 0;

  Button4.OwnerScreen     = &Screen2;
  Button4.Order           = 4;
  Button4.Left            = 280;
  Button4.Top             = 152;
  Button4.Width           = 70;
  Button4.Height          = 50;
  Button4.Pen_Width       = 1;
  Button4.Pen_Color       = 0xFFFF;
  Button4.Visible         = 1;
  Button4.Active          = 1;
  Button4.Transparent     = 1;
  Button4.Caption         = Button4_Caption;
  Button4.TextAlign       = _taCenter;
  Button4.TextAlignVertical= _tavMiddle;
  Button4.FontName        = Tahoma11x13_Regular;
  Button4.PressColEnabled = 1;
  Button4.Font_Color      = 0xFFFF;
  Button4.VerticalText    = 0;
  Button4.Gradient        = 1;
  Button4.Gradient_Orientation = 0;
  Button4.Gradient_Start_Color = 0x0000;
  Button4.Gradient_End_Color = 0x0000;
  Button4.Color           = 0x0000;
  Button4.Press_Color     = 0xE71C;
  Button4.OnUpPtr         = 0;
  Button4.OnDownPtr       = 0;
  Button4.OnClickPtr      = Button4OnClick;
  Button4.OnPressPtr      = 0;

  Button5.OwnerScreen     = &Screen2;
  Button5.Order           = 5;
  Button5.Left            = 358;
  Button5.Top             = 152;
  Button5.Width           = 70;
  Button5.Height          = 50;
  Button5.Pen_Width       = 1;
  Button5.Pen_Color       = 0xFFFF;
  Button5.Visible         = 1;
  Button5.Active          = 1;
  Button5.Transparent     = 1;
  Button5.Caption         = Button5_Caption;
  Button5.TextAlign       = _taCenter;
  Button5.TextAlignVertical= _tavMiddle;
  Button5.FontName        = Tahoma11x13_Regular;
  Button5.PressColEnabled = 1;
  Button5.Font_Color      = 0xFFFF;
  Button5.VerticalText    = 0;
  Button5.Gradient        = 1;
  Button5.Gradient_Orientation = 0;
  Button5.Gradient_Start_Color = 0x0000;
  Button5.Gradient_End_Color = 0x0000;
  Button5.Color           = 0x0000;
  Button5.Press_Color     = 0xE71C;
  Button5.OnUpPtr         = 0;
  Button5.OnDownPtr       = 0;
  Button5.OnClickPtr      = Button5OnClick;
  Button5.OnPressPtr      = 0;

  Button6.OwnerScreen     = &Screen2;
  Button6.Order           = 6;
  Button6.Left            = 39;
  Button6.Top             = 208;
  Button6.Width           = 70;
  Button6.Height          = 50;
  Button6.Pen_Width       = 1;
  Button6.Pen_Color       = 0xFFFF;
  Button6.Visible         = 1;
  Button6.Active          = 1;
  Button6.Transparent     = 1;
  Button6.Caption         = Button6_Caption;
  Button6.TextAlign       = _taCenter;
  Button6.TextAlignVertical= _tavMiddle;
  Button6.FontName        = Tahoma11x13_Regular;
  Button6.PressColEnabled = 1;
  Button6.Font_Color      = 0xFFFF;
  Button6.VerticalText    = 0;
  Button6.Gradient        = 1;
  Button6.Gradient_Orientation = 0;
  Button6.Gradient_Start_Color = 0x0000;
  Button6.Gradient_End_Color = 0x0000;
  Button6.Color           = 0x0000;
  Button6.Press_Color     = 0xE71C;
  Button6.OnUpPtr         = 0;
  Button6.OnDownPtr       = 0;
  Button6.OnClickPtr      = Button6OnClick;
  Button6.OnPressPtr      = 0;

  Button7.OwnerScreen     = &Screen2;
  Button7.Order           = 7;
  Button7.Left            = 119;
  Button7.Top             = 208;
  Button7.Width           = 70;
  Button7.Height          = 50;
  Button7.Pen_Width       = 1;
  Button7.Pen_Color       = 0xFFFF;
  Button7.Visible         = 1;
  Button7.Active          = 1;
  Button7.Transparent     = 1;
  Button7.Caption         = Button7_Caption;
  Button7.TextAlign       = _taCenter;
  Button7.TextAlignVertical= _tavMiddle;
  Button7.FontName        = Tahoma11x13_Regular;
  Button7.PressColEnabled = 1;
  Button7.Font_Color      = 0xFFFF;
  Button7.VerticalText    = 0;
  Button7.Gradient        = 1;
  Button7.Gradient_Orientation = 0;
  Button7.Gradient_Start_Color = 0x0000;
  Button7.Gradient_End_Color = 0x0000;
  Button7.Color           = 0x0000;
  Button7.Press_Color     = 0xE71C;
  Button7.OnUpPtr         = 0;
  Button7.OnDownPtr       = 0;
  Button7.OnClickPtr      = Button7OnClick;
  Button7.OnPressPtr      = 0;

  Button8.OwnerScreen     = &Screen2;
  Button8.Order           = 8;
  Button8.Left            = 200;
  Button8.Top             = 208;
  Button8.Width           = 70;
  Button8.Height          = 50;
  Button8.Pen_Width       = 1;
  Button8.Pen_Color       = 0xFFFF;
  Button8.Visible         = 1;
  Button8.Active          = 1;
  Button8.Transparent     = 1;
  Button8.Caption         = Button8_Caption;
  Button8.TextAlign       = _taCenter;
  Button8.TextAlignVertical= _tavMiddle;
  Button8.FontName        = Tahoma11x13_Regular;
  Button8.PressColEnabled = 1;
  Button8.Font_Color      = 0xFFFF;
  Button8.VerticalText    = 0;
  Button8.Gradient        = 1;
  Button8.Gradient_Orientation = 0;
  Button8.Gradient_Start_Color = 0x0000;
  Button8.Gradient_End_Color = 0x0000;
  Button8.Color           = 0x0000;
  Button8.Press_Color     = 0xE71C;
  Button8.OnUpPtr         = 0;
  Button8.OnDownPtr       = 0;
  Button8.OnClickPtr      = Button8OnClick;
  Button8.OnPressPtr      = 0;

  Button9.OwnerScreen     = &Screen2;
  Button9.Order           = 9;
  Button9.Left            = 279;
  Button9.Top             = 208;
  Button9.Width           = 70;
  Button9.Height          = 50;
  Button9.Pen_Width       = 1;
  Button9.Pen_Color       = 0xFFFF;
  Button9.Visible         = 1;
  Button9.Active          = 1;
  Button9.Transparent     = 1;
  Button9.Caption         = Button9_Caption;
  Button9.TextAlign       = _taCenter;
  Button9.TextAlignVertical= _tavMiddle;
  Button9.FontName        = Tahoma11x13_Regular;
  Button9.PressColEnabled = 1;
  Button9.Font_Color      = 0xFFFF;
  Button9.VerticalText    = 0;
  Button9.Gradient        = 1;
  Button9.Gradient_Orientation = 0;
  Button9.Gradient_Start_Color = 0x0000;
  Button9.Gradient_End_Color = 0x0000;
  Button9.Color           = 0x0000;
  Button9.Press_Color     = 0xE71C;
  Button9.OnUpPtr         = 0;
  Button9.OnDownPtr       = 0;
  Button9.OnClickPtr      = Button9OnClick;
  Button9.OnPressPtr      = 0;

  Button10.OwnerScreen     = &Screen2;
  Button10.Order           = 10;
  Button10.Left            = 358;
  Button10.Top             = 207;
  Button10.Width           = 70;
  Button10.Height          = 50;
  Button10.Pen_Width       = 1;
  Button10.Pen_Color       = 0xFFFF;
  Button10.Visible         = 1;
  Button10.Active          = 1;
  Button10.Transparent     = 1;
  Button10.Caption         = Button10_Caption;
  Button10.TextAlign       = _taCenter;
  Button10.TextAlignVertical= _tavMiddle;
  Button10.FontName        = Tahoma11x13_Regular;
  Button10.PressColEnabled = 1;
  Button10.Font_Color      = 0xFFFF;
  Button10.VerticalText    = 0;
  Button10.Gradient        = 1;
  Button10.Gradient_Orientation = 0;
  Button10.Gradient_Start_Color = 0x0000;
  Button10.Gradient_End_Color = 0x0000;
  Button10.Color           = 0x0000;
  Button10.Press_Color     = 0xE71C;
  Button10.OnUpPtr         = 0;
  Button10.OnDownPtr       = 0;
  Button10.OnClickPtr      = Button10OnClick;
  Button10.OnPressPtr      = 0;

  Box5.OwnerScreen     = &Screen2;
  Box5.Order           = 11;
  Box5.Left            = 116;
  Box5.Top             = 56;
  Box5.Width           = 63;
  Box5.Height          = 37;
  Box5.Pen_Width       = 1;
  Box5.Pen_Color       = 0x0000;
  Box5.Visible         = 1;
  Box5.Active          = 1;
  Box5.Transparent     = 1;
  Box5.Gradient        = 1;
  Box5.Gradient_Orientation = 0;
  Box5.Gradient_Start_Color = 0xFFFF;
  Box5.Gradient_End_Color = 0xFFFF;
  Box5.Color           = 0xFFFF;
  Box5.PressColEnabled = 1;
  Box5.Press_Color     = 0xE71C;
  Box5.OnUpPtr         = 0;
  Box5.OnDownPtr       = 0;
  Box5.OnClickPtr      = 0;
  Box5.OnPressPtr      = 0;

  hour1.OwnerScreen     = &Screen2;
  hour1.Order           = 12;
  hour1.Left            = 120;
  hour1.Top             = 56;
  hour1.Width           = 24;
  hour1.Height          = 36;
  hour1.Visible         = 1;
  hour1.Active          = 0;
  hour1.Caption         = hour1_Caption;
  hour1.FontName        = Tahoma26x33_Regular;
  hour1.Font_Color      = 0x0000;
  hour1.VerticalText    = 0;
  hour1.OnUpPtr         = 0;
  hour1.OnDownPtr       = 0;
  hour1.OnClickPtr      = 0;
  hour1.OnPressPtr      = 0;

  Box6.OwnerScreen     = &Screen2;
  Box6.Order           = 13;
  Box6.Left            = 202;
  Box6.Top             = 56;
  Box6.Width           = 63;
  Box6.Height          = 37;
  Box6.Pen_Width       = 1;
  Box6.Pen_Color       = 0x0000;
  Box6.Visible         = 1;
  Box6.Active          = 1;
  Box6.Transparent     = 1;
  Box6.Gradient        = 1;
  Box6.Gradient_Orientation = 0;
  Box6.Gradient_Start_Color = 0xFFFF;
  Box6.Gradient_End_Color = 0xFFFF;
  Box6.Color           = 0xFFFF;
  Box6.PressColEnabled = 1;
  Box6.Press_Color     = 0xE71C;
  Box6.OnUpPtr         = 0;
  Box6.OnDownPtr       = 0;
  Box6.OnClickPtr      = 0;
  Box6.OnPressPtr      = 0;

  minute1.OwnerScreen     = &Screen2;
  minute1.Order           = 14;
  minute1.Left            = 210;
  minute1.Top             = 56;
  minute1.Width           = 24;
  minute1.Height          = 36;
  minute1.Visible         = 1;
  minute1.Active          = 0;
  minute1.Caption         = minute1_Caption;
  minute1.FontName        = Tahoma26x33_Regular;
  minute1.Font_Color      = 0x0000;
  minute1.VerticalText    = 0;
  minute1.OnUpPtr         = 0;
  minute1.OnDownPtr       = 0;
  minute1.OnClickPtr      = 0;
  minute1.OnPressPtr      = 0;

  Box7.OwnerScreen     = &Screen2;
  Box7.Order           = 15;
  Box7.Left            = 293;
  Box7.Top             = 57;
  Box7.Width           = 63;
  Box7.Height          = 37;
  Box7.Pen_Width       = 1;
  Box7.Pen_Color       = 0x0000;
  Box7.Visible         = 1;
  Box7.Active          = 1;
  Box7.Transparent     = 1;
  Box7.Gradient        = 1;
  Box7.Gradient_Orientation = 0;
  Box7.Gradient_Start_Color = 0xFFFF;
  Box7.Gradient_End_Color = 0xFFFF;
  Box7.Color           = 0xFFFF;
  Box7.PressColEnabled = 1;
  Box7.Press_Color     = 0xE71C;
  Box7.OnUpPtr         = 0;
  Box7.OnDownPtr       = 0;
  Box7.OnClickPtr      = 0;
  Box7.OnPressPtr      = 0;

  sec1.OwnerScreen     = &Screen2;
  sec1.Order           = 16;
  sec1.Left            = 300;
  sec1.Top             = 56;
  sec1.Width           = 24;
  sec1.Height          = 36;
  sec1.Visible         = 1;
  sec1.Active          = 0;
  sec1.Caption         = sec1_Caption;
  sec1.FontName        = Tahoma26x33_Regular;
  sec1.Font_Color      = 0x0000;
  sec1.VerticalText    = 0;
  sec1.OnUpPtr         = 0;
  sec1.OnDownPtr       = 0;
  sec1.OnClickPtr      = 0;
  sec1.OnPressPtr      = 0;

  Label22.OwnerScreen     = &Screen2;
  Label22.Order           = 17;
  Label22.Left            = 181;
  Label22.Top             = 56;
  Label22.Width           = 19;
  Label22.Height          = 36;
  Label22.Visible         = 1;
  Label22.Active          = 1;
  Label22.Caption         = Label22_Caption;
  Label22.FontName        = Tahoma32x33_Bold;
  Label22.Font_Color      = 0x0000;
  Label22.VerticalText    = 0;
  Label22.OnUpPtr         = 0;
  Label22.OnDownPtr       = 0;
  Label22.OnClickPtr      = 0;
  Label22.OnPressPtr      = 0;

  Label23.OwnerScreen     = &Screen2;
  Label23.Order           = 18;
  Label23.Left            = 269;
  Label23.Top             = 57;
  Label23.Width           = 19;
  Label23.Height          = 36;
  Label23.Visible         = 1;
  Label23.Active          = 1;
  Label23.Caption         = Label23_Caption;
  Label23.FontName        = Tahoma32x33_Bold;
  Label23.Font_Color      = 0x0000;
  Label23.VerticalText    = 0;
  Label23.OnUpPtr         = 0;
  Label23.OnDownPtr       = 0;
  Label23.OnClickPtr      = 0;
  Label23.OnPressPtr      = 0;

  Box8.OwnerScreen     = &Screen2;
  Box8.Order           = 19;
  Box8.Left            = 115;
  Box8.Top             = 105;
  Box8.Width           = 63;
  Box8.Height          = 37;
  Box8.Pen_Width       = 1;
  Box8.Pen_Color       = 0x0000;
  Box8.Visible         = 1;
  Box8.Active          = 1;
  Box8.Transparent     = 1;
  Box8.Gradient        = 1;
  Box8.Gradient_Orientation = 0;
  Box8.Gradient_Start_Color = 0xFFFF;
  Box8.Gradient_End_Color = 0xFFFF;
  Box8.Color           = 0xFFFF;
  Box8.PressColEnabled = 1;
  Box8.Press_Color     = 0xE71C;
  Box8.OnUpPtr         = 0;
  Box8.OnDownPtr       = 0;
  Box8.OnClickPtr      = 0;
  Box8.OnPressPtr      = 0;

  date1.OwnerScreen     = &Screen2;
  date1.Order           = 20;
  date1.Left            = 120;
  date1.Top             = 105;
  date1.Width           = 24;
  date1.Height          = 36;
  date1.Visible         = 1;
  date1.Active          = 0;
  date1.Caption         = date1_Caption;
  date1.FontName        = Tahoma26x33_Regular;
  date1.Font_Color      = 0x0000;
  date1.VerticalText    = 0;
  date1.OnUpPtr         = 0;
  date1.OnDownPtr       = 0;
  date1.OnClickPtr      = 0;
  date1.OnPressPtr      = 0;

  Box9.OwnerScreen     = &Screen2;
  Box9.Order           = 21;
  Box9.Left            = 201;
  Box9.Top             = 106;
  Box9.Width           = 63;
  Box9.Height          = 37;
  Box9.Pen_Width       = 1;
  Box9.Pen_Color       = 0x0000;
  Box9.Visible         = 1;
  Box9.Active          = 1;
  Box9.Transparent     = 1;
  Box9.Gradient        = 1;
  Box9.Gradient_Orientation = 0;
  Box9.Gradient_Start_Color = 0xFFFF;
  Box9.Gradient_End_Color = 0xFFFF;
  Box9.Color           = 0xFFFF;
  Box9.PressColEnabled = 1;
  Box9.Press_Color     = 0xE71C;
  Box9.OnUpPtr         = 0;
  Box9.OnDownPtr       = 0;
  Box9.OnClickPtr      = 0;
  Box9.OnPressPtr      = 0;

  month1.OwnerScreen     = &Screen2;
  month1.Order           = 22;
  month1.Left            = 210;
  month1.Top             = 105;
  month1.Width           = 24;
  month1.Height          = 36;
  month1.Visible         = 1;
  month1.Active          = 0;
  month1.Caption         = month1_Caption;
  month1.FontName        = Tahoma26x33_Regular;
  month1.Font_Color      = 0x0000;
  month1.VerticalText    = 0;
  month1.OnUpPtr         = 0;
  month1.OnDownPtr       = 0;
  month1.OnClickPtr      = 0;
  month1.OnPressPtr      = 0;

  Label30.OwnerScreen     = &Screen2;
  Label30.Order           = 23;
  Label30.Left            = 180;
  Label30.Top             = 107;
  Label30.Width           = 18;
  Label30.Height          = 36;
  Label30.Visible         = 1;
  Label30.Active          = 1;
  Label30.Caption         = Label30_Caption;
  Label30.FontName        = Tahoma32x33_Bold;
  Label30.Font_Color      = 0x0000;
  Label30.VerticalText    = 0;
  Label30.OnUpPtr         = 0;
  Label30.OnDownPtr       = 0;
  Label30.OnClickPtr      = 0;
  Label30.OnPressPtr      = 0;

  Label31.OwnerScreen     = &Screen2;
  Label31.Order           = 24;
  Label31.Left            = 269;
  Label31.Top             = 107;
  Label31.Width           = 18;
  Label31.Height          = 36;
  Label31.Visible         = 1;
  Label31.Active          = 1;
  Label31.Caption         = Label31_Caption;
  Label31.FontName        = Tahoma32x33_Bold;
  Label31.Font_Color      = 0x0000;
  Label31.VerticalText    = 0;
  Label31.OnUpPtr         = 0;
  Label31.OnDownPtr       = 0;
  Label31.OnClickPtr      = 0;
  Label31.OnPressPtr      = 0;

  Box11.OwnerScreen     = &Screen2;
  Box11.Order           = 25;
  Box11.Left            = 292;
  Box11.Top             = 103;
  Box11.Width           = 63;
  Box11.Height          = 37;
  Box11.Pen_Width       = 1;
  Box11.Pen_Color       = 0x0000;
  Box11.Visible         = 1;
  Box11.Active          = 1;
  Box11.Transparent     = 1;
  Box11.Gradient        = 1;
  Box11.Gradient_Orientation = 0;
  Box11.Gradient_Start_Color = 0xFFFF;
  Box11.Gradient_End_Color = 0xFFFF;
  Box11.Color           = 0xFFFF;
  Box11.PressColEnabled = 1;
  Box11.Press_Color     = 0xE71C;
  Box11.OnUpPtr         = 0;
  Box11.OnDownPtr       = 0;
  Box11.OnClickPtr      = 0;
  Box11.OnPressPtr      = 0;

  year1.OwnerScreen     = &Screen2;
  year1.Order           = 26;
  year1.Left            = 300;
  year1.Top             = 105;
  year1.Width           = 24;
  year1.Height          = 36;
  year1.Visible         = 1;
  year1.Active          = 0;
  year1.Caption         = year1_Caption;
  year1.FontName        = Tahoma26x33_Regular;
  year1.Font_Color      = 0x0000;
  year1.VerticalText    = 0;
  year1.OnUpPtr         = 0;
  year1.OnDownPtr       = 0;
  year1.OnClickPtr      = 0;
  year1.OnPressPtr      = 0;

  Label17.OwnerScreen     = &Screen2;
  Label17.Order           = 27;
  Label17.Left            = 145;
  Label17.Top             = 56;
  Label17.Width           = 24;
  Label17.Height          = 36;
  Label17.Visible         = 1;
  Label17.Active          = 0;
  Label17.Caption         = Label17_Caption;
  Label17.FontName        = Tahoma26x33_Regular;
  Label17.Font_Color      = 0x0000;
  Label17.VerticalText    = 0;
  Label17.OnUpPtr         = 0;
  Label17.OnDownPtr       = 0;
  Label17.OnClickPtr      = 0;
  Label17.OnPressPtr      = 0;

  Label18.OwnerScreen     = &Screen2;
  Label18.Order           = 28;
  Label18.Left            = 145;
  Label18.Top             = 105;
  Label18.Width           = 24;
  Label18.Height          = 36;
  Label18.Visible         = 1;
  Label18.Active          = 0;
  Label18.Caption         = Label18_Caption;
  Label18.FontName        = Tahoma26x33_Regular;
  Label18.Font_Color      = 0x0000;
  Label18.VerticalText    = 0;
  Label18.OnUpPtr         = 0;
  Label18.OnDownPtr       = 0;
  Label18.OnClickPtr      = 0;
  Label18.OnPressPtr      = 0;

  Label19.OwnerScreen     = &Screen2;
  Label19.Order           = 29;
  Label19.Left            = 235;
  Label19.Top             = 56;
  Label19.Width           = 24;
  Label19.Height          = 36;
  Label19.Visible         = 1;
  Label19.Active          = 0;
  Label19.Caption         = Label19_Caption;
  Label19.FontName        = Tahoma26x33_Regular;
  Label19.Font_Color      = 0x0000;
  Label19.VerticalText    = 0;
  Label19.OnUpPtr         = 0;
  Label19.OnDownPtr       = 0;
  Label19.OnClickPtr      = 0;
  Label19.OnPressPtr      = 0;

  Label20.OwnerScreen     = &Screen2;
  Label20.Order           = 30;
  Label20.Left            = 325;
  Label20.Top             = 56;
  Label20.Width           = 24;
  Label20.Height          = 36;
  Label20.Visible         = 1;
  Label20.Active          = 0;
  Label20.Caption         = Label20_Caption;
  Label20.FontName        = Tahoma26x33_Regular;
  Label20.Font_Color      = 0x0000;
  Label20.VerticalText    = 0;
  Label20.OnUpPtr         = 0;
  Label20.OnDownPtr       = 0;
  Label20.OnClickPtr      = 0;
  Label20.OnPressPtr      = 0;

  Label21.OwnerScreen     = &Screen2;
  Label21.Order           = 31;
  Label21.Left            = 235;
  Label21.Top             = 105;
  Label21.Width           = 24;
  Label21.Height          = 36;
  Label21.Visible         = 1;
  Label21.Active          = 0;
  Label21.Caption         = Label21_Caption;
  Label21.FontName        = Tahoma26x33_Regular;
  Label21.Font_Color      = 0x0000;
  Label21.VerticalText    = 0;
  Label21.OnUpPtr         = 0;
  Label21.OnDownPtr       = 0;
  Label21.OnClickPtr      = 0;
  Label21.OnPressPtr      = 0;

  Label24.OwnerScreen     = &Screen2;
  Label24.Order           = 32;
  Label24.Left            = 325;
  Label24.Top             = 105;
  Label24.Width           = 24;
  Label24.Height          = 36;
  Label24.Visible         = 1;
  Label24.Active          = 0;
  Label24.Caption         = Label24_Caption;
  Label24.FontName        = Tahoma26x33_Regular;
  Label24.Font_Color      = 0x0000;
  Label24.VerticalText    = 0;
  Label24.OnUpPtr         = 0;
  Label24.OnDownPtr       = 0;
  Label24.OnClickPtr      = 0;
  Label24.OnPressPtr      = 0;

  Label25.OwnerScreen     = &Screen2;
  Label25.Order           = 33;
  Label25.Left            = 21;
  Label25.Top             = 60;
  Label25.Width           = 80;
  Label25.Height          = 35;
  Label25.Visible         = 1;
  Label25.Active          = 0;
  Label25.Caption         = Label25_Caption;
  Label25.FontName        = Segoe_UI_Semibold23x32_Bold;
  Label25.Font_Color      = 0x0000;
  Label25.VerticalText    = 0;
  Label25.OnUpPtr         = 0;
  Label25.OnDownPtr       = 0;
  Label25.OnClickPtr      = 0;
  Label25.OnPressPtr      = 0;

  Label26.OwnerScreen     = &Screen2;
  Label26.Order           = 34;
  Label26.Left            = 21;
  Label26.Top             = 106;
  Label26.Width           = 77;
  Label26.Height          = 35;
  Label26.Visible         = 1;
  Label26.Active          = 0;
  Label26.Caption         = Label26_Caption;
  Label26.FontName        = Segoe_UI_Semibold23x32_Bold;
  Label26.Font_Color      = 0x0000;
  Label26.VerticalText    = 0;
  Label26.OnUpPtr         = 0;
  Label26.OnDownPtr       = 0;
  Label26.OnClickPtr      = 0;
  Label26.OnPressPtr      = 0;

  Box10.OwnerScreen     = &Screen2;
  Box10.Order           = 35;
  Box10.Left            = 369;
  Box10.Top             = 82;
  Box10.Width           = 102;
  Box10.Height          = 37;
  Box10.Pen_Width       = 1;
  Box10.Pen_Color       = 0x0000;
  Box10.Visible         = 1;
  Box10.Active          = 0;
  Box10.Transparent     = 1;
  Box10.Gradient        = 1;
  Box10.Gradient_Orientation = 0;
  Box10.Gradient_Start_Color = 0xFFFF;
  Box10.Gradient_End_Color = 0xFFFF;
  Box10.Color           = 0xFFFF;
  Box10.PressColEnabled = 1;
  Box10.Press_Color     = 0xE71C;
  Box10.OnUpPtr         = 0;
  Box10.OnDownPtr       = 0;
  Box10.OnClickPtr      = 0;
  Box10.OnPressPtr      = 0;

  Label4.OwnerScreen     = &Screen2;
  Label4.Order           = 36;
  Label4.Left            = 374;
  Label4.Top             = 48;
  Label4.Width           = 89;
  Label4.Height          = 31;
  Label4.Visible         = 1;
  Label4.Active          = 0;
  Label4.Caption         = Label4_Caption;
  Label4.FontName        = Segoe_UI_Semibold20x30_Bold;
  Label4.Font_Color      = 0x0000;
  Label4.VerticalText    = 0;
  Label4.OnUpPtr         = 0;
  Label4.OnDownPtr       = 0;
  Label4.OnClickPtr      = 0;
  Label4.OnPressPtr      = 0;

  Alarm0.OwnerScreen     = &Screen2;
  Alarm0.Order           = 37;
  Alarm0.Left            = 370;
  Alarm0.Top             = 82;
  Alarm0.Width           = 24;
  Alarm0.Height          = 36;
  Alarm0.Visible         = 1;
  Alarm0.Active          = 0;
  Alarm0.Caption         = Alarm0_Caption;
  Alarm0.FontName        = Tahoma26x33_Regular;
  Alarm0.Font_Color      = 0x0000;
  Alarm0.VerticalText    = 0;
  Alarm0.OnUpPtr         = 0;
  Alarm0.OnDownPtr       = 0;
  Alarm0.OnClickPtr      = 0;
  Alarm0.OnPressPtr      = 0;

  Alarm1.OwnerScreen     = &Screen2;
  Alarm1.Order           = 38;
  Alarm1.Left            = 395;
  Alarm1.Top             = 82;
  Alarm1.Width           = 24;
  Alarm1.Height          = 36;
  Alarm1.Visible         = 1;
  Alarm1.Active          = 0;
  Alarm1.Caption         = Alarm1_Caption;
  Alarm1.FontName        = Tahoma26x33_Regular;
  Alarm1.Font_Color      = 0x0000;
  Alarm1.VerticalText    = 0;
  Alarm1.OnUpPtr         = 0;
  Alarm1.OnDownPtr       = 0;
  Alarm1.OnClickPtr      = 0;
  Alarm1.OnPressPtr      = 0;

  Alarm2.OwnerScreen     = &Screen2;
  Alarm2.Order           = 39;
  Alarm2.Left            = 420;
  Alarm2.Top             = 82;
  Alarm2.Width           = 24;
  Alarm2.Height          = 36;
  Alarm2.Visible         = 1;
  Alarm2.Active          = 0;
  Alarm2.Caption         = Alarm2_Caption;
  Alarm2.FontName        = Tahoma26x33_Regular;
  Alarm2.Font_Color      = 0x0000;
  Alarm2.VerticalText    = 0;
  Alarm2.OnUpPtr         = 0;
  Alarm2.OnDownPtr       = 0;
  Alarm2.OnClickPtr      = 0;
  Alarm2.OnPressPtr      = 0;

  Alarm3.OwnerScreen     = &Screen2;
  Alarm3.Order           = 40;
  Alarm3.Left            = 445;
  Alarm3.Top             = 82;
  Alarm3.Width           = 24;
  Alarm3.Height          = 36;
  Alarm3.Visible         = 1;
  Alarm3.Active          = 0;
  Alarm3.Caption         = Alarm3_Caption;
  Alarm3.FontName        = Tahoma26x33_Regular;
  Alarm3.Font_Color      = 0x0000;
  Alarm3.VerticalText    = 0;
  Alarm3.OnUpPtr         = 0;
  Alarm3.OnDownPtr       = 0;
  Alarm3.OnClickPtr      = 0;
  Alarm3.OnPressPtr      = 0;
}

static char IsInsideObject (unsigned int X, unsigned int Y, unsigned int Left, unsigned int Top, unsigned int Width, unsigned int Height) { // static
  if ( (Left<= X) && (Left+ Width - 1 >= X) &&
       (Top <= Y)  && (Top + Height - 1 >= Y) )
    return 1;
  else
    return 0;
}


#define GetButton(index)              CurrentScreen->Buttons[index]
#define GetLabel(index)               CurrentScreen->Labels[index]
#define GetBox(index)                 CurrentScreen->Boxes[index]
#define GetLine(index)                CurrentScreen->Lines[index]

void DrawButton(TButton *Abutton) {
unsigned int ALeft, ATop;
  if (Abutton->Visible != 0) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(Abutton->Transparent, Abutton->Press_Color, Abutton->Gradient, Abutton->Gradient_Orientation, Abutton->Gradient_End_Color, Abutton->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(Abutton->Transparent, Abutton->Color, Abutton->Gradient, Abutton->Gradient_Orientation, Abutton->Gradient_Start_Color, Abutton->Gradient_End_Color);
    }
    TFT_Set_Pen(Abutton->Pen_Color, Abutton->Pen_Width);
    TFT_Rectangle(Abutton->Left, Abutton->Top, Abutton->Left + Abutton->Width - 1, Abutton->Top + Abutton->Height - 1);
    if (Abutton->VerticalText != 0)
      TFT_Set_Font(Abutton->FontName, Abutton->Font_Color, FO_VERTICAL_COLUMN);
    else
      TFT_Set_Font(Abutton->FontName, Abutton->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text_Return_Pos(Abutton->Caption, Abutton->Left, Abutton->Top);
    if (Abutton->TextAlign == _taLeft)
      ALeft = Abutton->Left + 4;
    else if (Abutton->TextAlign == _taCenter)
      ALeft = Abutton->Left + (Abutton->Width - caption_length) / 2;
    else if (Abutton->TextAlign == _taRight)
      ALeft = Abutton->Left + Abutton->Width - caption_length - 4;

    if (Abutton->TextAlignVertical == _tavTop)
      ATop = Abutton->Top + 4;
    else if (Abutton->TextAlignVertical == _tavMiddle)
      ATop = Abutton->Top + ((Abutton->Height - caption_height) / 2);
    else if (Abutton->TextAlignVertical == _tavBottom)
      ATop = Abutton->Top + (Abutton->Height - caption_height - 4);

    TFT_Write_Text(Abutton->Caption, ALeft, ATop);
  }
}

void DrawLabel(TLabel *ALabel) {
  if (ALabel->Visible != 0) {
    if (ALabel->VerticalText != 0)
      TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_VERTICAL_COLUMN);
    else
      TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text(ALabel->Caption, ALabel->Left, ALabel->Top);
  }
}

void DrawBox(TBox *ABox) {
  if (ABox->Visible != 0) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(ABox->Transparent, ABox->Press_Color, ABox->Gradient, ABox->Gradient_Orientation, ABox->Gradient_End_Color, ABox->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(ABox->Transparent, ABox->Color, ABox->Gradient, ABox->Gradient_Orientation, ABox->Gradient_Start_Color, ABox->Gradient_End_Color);
    }
    TFT_Set_Pen(ABox->Pen_Color, ABox->Pen_Width);
    TFT_Rectangle(ABox->Left, ABox->Top, ABox->Left + ABox->Width - 1, ABox->Top + ABox->Height - 1);
  }
}

void DrawLine(TLine *Aline) {
  if (Aline->Visible != 0) {
    TFT_Set_Pen(Aline->Color, Aline->Pen_Width);
    TFT_Line(Aline->First_Point_X, Aline->First_Point_Y, Aline->Second_Point_X, Aline->Second_Point_Y);
  }
}

void DrawScreen(TScreen *aScreen) {
 unsigned int order;
  unsigned short button_idx;
  TButton *local_button;
  unsigned short label_idx;
  TLabel *local_label;
  unsigned short box_idx;
  TBox *local_box;
  unsigned short line_idx;
  TLine *local_line;
  char save_bled;

  object_pressed = 0;
  order = 0;
  button_idx = 0;
  label_idx = 0;
  box_idx = 0;
  line_idx = 0;
  CurrentScreen = aScreen;

  if ((display_width != CurrentScreen->Width) || (display_height != CurrentScreen->Height)) {
    save_bled = TFT_BLED;
    TFT_BLED           = 0;
    TFT_Set_Active(Set_Index, Write_Command, Write_Data);
    TFT_Init_SSD1963(CurrentScreen->Width, CurrentScreen->Height);
    STMPE610_SetSize(CurrentScreen->Width, CurrentScreen->Height);
    TFT_Fill_Screen(0xFFFF);
    TFT_Set_DBC_SSD1963(255);
    display_width = CurrentScreen->Width;
    display_height = CurrentScreen->Height;
    TFT_BLED           = save_bled;
  }
  else
    TFT_Fill_Screen(0xFFFF);


  while (order < CurrentScreen->ObjectsCount) {
    if (button_idx < CurrentScreen->ButtonsCount) {
      local_button = GetButton(button_idx);
      if (order == local_button->Order) {
        button_idx++;
        order++;
        DrawButton(local_button);
      }
    }

    if (label_idx < CurrentScreen->LabelsCount) {
      local_label = GetLabel(label_idx);
      if (order == local_label->Order) {
        label_idx++;
        order++;
        DrawLabel(local_label);
      }
    }

    if (box_idx < CurrentScreen->BoxesCount) {
      local_box = GetBox(box_idx);
      if (order == local_box->Order) {
        box_idx++;
        order++;
        DrawBox(local_box);
      }
    }

    if (line_idx < CurrentScreen->LinesCount) {
      local_line = GetLine(line_idx);
      if (order == local_line->Order) {
        line_idx++;
        order++;
        DrawLine(local_line);
      }
    }

  }
}

void Get_Object(unsigned int X, unsigned int Y) {
  button_order        = -1;
  label_order         = -1;
  box_order           = -1;
  //  Buttons
  for ( _object_count = 0 ; _object_count < CurrentScreen->ButtonsCount ; _object_count++ ) {
    local_button = GetButton(_object_count);
    if (local_button->Active != 0) {
      if (IsInsideObject(X, Y, local_button->Left, local_button->Top,
                         local_button->Width, local_button->Height) == 1) {
        button_order = local_button->Order;
        exec_button = local_button;
      }
    }
  }

  //  Labels
  for ( _object_count = 0 ; _object_count < CurrentScreen->LabelsCount ; _object_count++ ) {
    local_label = GetLabel(_object_count);
    if (local_label->Active != 0) {
      if (IsInsideObject(X, Y, local_label->Left, local_label->Top,
                         local_label->Width, local_label->Height) == 1) {
        label_order = local_label->Order;
        exec_label = local_label;
      }
    }
  }

  //  Boxes
  for ( _object_count = 0 ; _object_count < CurrentScreen->BoxesCount ; _object_count++ ) {
    local_box = GetBox(_object_count);
    if (local_box->Active != 0) {
      if (IsInsideObject(X, Y, local_box->Left, local_box->Top,
                         local_box->Width, local_box->Height) == 1) {
        box_order = local_box->Order;
        exec_box = local_box;
      }
    }
  }

  _object_count = -1;
  if (button_order >  _object_count )
    _object_count = button_order;
  if (label_order >  _object_count )
    _object_count = label_order;
  if (box_order >  _object_count )
    _object_count = box_order;
}


void Process_TP_Press(unsigned int X, unsigned int Y) {
  exec_button         = 0;
  exec_label          = 0;
  exec_box            = 0;

  Get_Object(X, Y);

  if (_object_count != -1) {
    if (_object_count == button_order) {
      if (exec_button->Active != 0) {
        if (exec_button->OnPressPtr != 0) {
          exec_button->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active != 0) {
        if (exec_label->OnPressPtr != 0) {
          exec_label->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == box_order) {
      if (exec_box->Active != 0) {
        if (exec_box->OnPressPtr != 0) {
          exec_box->OnPressPtr();
          return;
        }
      }
    }

  }
}

void Process_TP_Up(unsigned int X, unsigned int Y) {

  switch (PressedObjectType) {
    // Button
    case 0: {
      if (PressedObject != 0) {
        exec_button = (TButton*)PressedObject;
        if ((exec_button->PressColEnabled != 0) && (exec_button->OwnerScreen == CurrentScreen)) {
          DrawButton(exec_button);
        }
        break;
      }
      break;
    }
    // Box
    case 6: {
      if (PressedObject != 0) {
        exec_box = (TBox*)PressedObject;
        if ((exec_box->PressColEnabled != 0) && (exec_box->OwnerScreen == CurrentScreen)) {
          DrawBox(exec_box);
        }
        break;
      }
      break;
    }
  }

  exec_label          = 0;

  Get_Object(X, Y);


  if (_object_count != -1) {
  // Buttons
    if (_object_count == button_order) {
      if (exec_button->Active != 0) {
        if (exec_button->OnUpPtr != 0)
          exec_button->OnUpPtr();
        if (PressedObject == (void *)exec_button)
          if (exec_button->OnClickPtr != 0)
            exec_button->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Labels
    if (_object_count == label_order) {
      if (exec_label->Active != 0) {
        if (exec_label->OnUpPtr != 0)
          exec_label->OnUpPtr();
        if (PressedObject == (void *)exec_label)
          if (exec_label->OnClickPtr != 0)
            exec_label->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Boxes
    if (_object_count == box_order) {
      if (exec_box->Active != 0) {
        if (exec_box->OnUpPtr != 0)
          exec_box->OnUpPtr();
        if (PressedObject == (void *)exec_box)
          if (exec_box->OnClickPtr != 0)
            exec_box->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  }
  PressedObject = 0;
  PressedObjectType = -1;
}

void Process_TP_Down(unsigned int X, unsigned int Y) {

  object_pressed      = 0;
  exec_button         = 0;
  exec_label          = 0;
  exec_box            = 0;

  Get_Object(X, Y);

  if (_object_count != -1) {
    if (_object_count == button_order) {
      if (exec_button->Active != 0) {
        if (exec_button->PressColEnabled != 0) {
          object_pressed = 1;
          DrawButton(exec_button);
        }
        PressedObject = (void *)exec_button;
        PressedObjectType = 0;
        if (exec_button->OnDownPtr != 0) {
          exec_button->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active != 0) {
        PressedObject = (void *)exec_label;
        PressedObjectType = 2;
        if (exec_label->OnDownPtr != 0) {
          exec_label->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == box_order) {
      if (exec_box->Active != 0) {
        if (exec_box->PressColEnabled != 0) {
          object_pressed = 1;
          DrawBox(exec_box);
        }
        PressedObject = (void *)exec_box;
        PressedObjectType = 6;
        if (exec_box->OnDownPtr != 0) {
          exec_box->OnDownPtr();
          return;
        }
      }
    }

  }
}

void Check_TP() {
  if (STMPE610_PressDetect()) {
    if (STMPE610_GetLastCoordinates(&Xcoord, &Ycoord) == 0) {
    // After a PRESS is detected read X-Y and convert it to Display dimensions space
      Process_TP_Press(Xcoord, Ycoord);
      if (PenDown == 0) {
        PenDown = 1;
        Process_TP_Down(Xcoord, Ycoord);
      }
    }
  }
  else if (PenDown == 1) {
    PenDown = 0;
    Process_TP_Up(Xcoord, Ycoord);
  }
}

void Init_MCU() {
  // Place your code here

  // If bus is busy wait SDA high before initializing I2C module
  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_6);
  GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_7);
  GPIOB_ODR.B6 = 1;
  while (GPIOB_IDR.B7 == 0) {
    GPIOB_ODR.B6 = 0;
    Delay_us(10);
    GPIOB_ODR.B6 = 1;
    Delay_us(10);
  }
  I2C1_Init_Advanced(400000, &_GPIO_MODULE_I2C1_PB67);

  TFT_Set_Default_Mode();
  GPIO_Digital_Output(&GPIOG_BASE, 0x00FF);
  GPIO_Digital_Output(&GPIOE_BASE, 0xFF00);
  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_4);
  GPIOB_ODR.B4 = 0;
  TFT_Set_Default_Mode();
  TFT_Set_MM_Plus();
  STMPE610_SetDefaultMode();
}

char STMPE610_Config()  {
  STMPE610_SetI2CAddress(STMPE610_I2C_ADDR1);
  if (STMPE610_IsOperational() != 0){
    return STMPE610_IO_NOT_OPERATIONAL;
  }

  STMPE610_Reset();
  STMPE610_Module(STMPE610_MODULE_TS | STMPE610_MODULE_ADC, STMPE610_ENABLE);
  STMPE610_AlternateFunction(STMPE610_GPIO_PIN1, STMPE610_ENABLE);
  STMPE610_SetGPIOPin(STMPE610_GPIO_PIN1, 0);   // IN1 to "0" -> I2C communication
  STMPE610_SetSize(480,272);
  STMPE610_Module(STMPE610_MODULE_TS | STMPE610_MODULE_ADC, STMPE610_ENABLE);
  STMPE610_EnableInterrupt(STMPE610_IE_TOUCH_DET | STMPE610_IE_FIFO_TH | STMPE610_IE_FIFO_0FLOW, STMPE610_ENABLE);
  STMPE610_ConfigureInterrupt(STMPE610_INT_POLARITY_ACTIVE_HIGH | STMPE610_INT_TYPE_EDGE | STMPE610_INT_ENABLE_ALL);
  STMPE610_SetADC(STMPE610_ADC_CTRL1_SAMPLETIME_44 | STMPE610_ADC_CTRL1_ADC_12BIT | STMPE610_ADC_CTRL1_INT_REFERENCE);
  Delay_10ms(); Delay_10ms();
  STMPE610_SetADCClock(STMPE610_ADC_CTRL2_3250_kHZ);
  STMPE610_AlternateFunction(STMPE610_GPIO_PIN4 | STMPE610_GPIO_PIN5 | STMPE610_GPIO_PIN6 | STMPE610_GPIO_PIN7, STMPE610_DISABLE);
  STMPE610_ConfigureTSC(STMPE610_TSC_CFG_AVE_CTRL_4S, STMPE610_TSC_CFG_TOUCH_DET_DELAY_500uS, STMPE610_TSC_CFG_TOUCH_SETTLING_500uS);
  STMPE610_SetFIFOThreshold(1);
  STMPE610_ResetFIFO();
  STMPE610_TSIDrive(STMPE610_TSC_I_DRIVE_20mA);
  STMPE610_TSControl(STMPE610_TSC_CTRL_TRACK0 | STMPE610_TSC_CTRL_ACQU_XYZ | STMPE610_TSC_CTRL_ENABLE);
  STMPE610_ZDataFraction(STMPE610_FRACP4_WHOLP4);
  STMPE610_SetTouchPressureThreshold(70);
  STMPE610_ClearInterrupts();
  STMPE610_WriteReg(STMPE610_INT_CTRL_REG, 0x01);
  return STMPE610_OK;
}


void Start_TP() {
  Init_MCU();

  InitializeTouchPanel();
  if (STMPE610_Config() == STMPE610_OK) {
  } else {
    while(1);
  }


  Delay_ms(1000);
  TFT_Fill_Screen(0);
  Calibrate();
  TFT_Fill_Screen(0);

  InitializeObjects();
  display_width = Screen1.Width;
  display_height = Screen1.Height;
  DrawScreen(&Screen2);
}
