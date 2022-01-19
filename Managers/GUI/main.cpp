#include <Core/../../GUI/Window.hpp>
#include "GUI.hpp"
#include <Core/../../GUI/ImGuiUtils.hpp>

int main()
{
    UGM::GUI::Window window;
    window.init("Untitled Linux Game Manager GUI", nullptr, UGM::Managers::GUI::render,  UGM::Managers::GUI::renderWindows);
    UGM::GUI::ImGuiUtils::init(window.data(), R"(
[Window][DockSpace Demo]
Pos=0,19
Size=800,581
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][Exit warning]
Pos=176,338
Size=505,122
Collapsed=0

[Window][About us]
Pos=8,259
Size=782,129
Collapsed=0

[Window][Show Directories]
Pos=439,270
Size=705,130
Collapsed=0

[Window][Main]
Pos=279,19
Size=521,581
Collapsed=0
DockId=0x00000002,0

[Window][Sidebar]
Pos=0,19
Size=277,581
Collapsed=0
DockId=0x00000001,0

[Window][New Warning]
Pos=233,326
Size=439,86
Collapsed=0

[Window][Pin]
Pos=291,261
Size=380,93
Collapsed=0

[Window][Delete]
Pos=640,216
Size=434,92
Collapsed=0

[Window][Poweroff Warning]
Pos=230,249
Size=361,86
Collapsed=0

[Window][Power off Warning]
Pos=711,291
Size=416,85
Collapsed=0

[Window][Power on Warning]
Pos=610,314
Size=415,139
Collapsed=0

[Window][Restart Warning]
Pos=114,260
Size=484,174
Collapsed=0

[Docking][Data]
DockSpace   ID=0x3BC79352 Window=0x4647B76E Pos=3120,49 Size=800,581 Split=X
  DockNode  ID=0x00000001 Parent=0x3BC79352 SizeRef=277,565 HiddenTabBar=1 Selected=0x495EF1C2
  DockNode  ID=0x00000002 Parent=0x3BC79352 SizeRef=521,565 CentralNode=1 HiddenTabBar=1 Selected=0x1F1A625A
)");
    while (!window.getWindowClose())
        window.tick();
}