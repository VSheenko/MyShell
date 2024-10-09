RequestExecutionLevel user
Outfile "MyShell_installer_win-x64.exe"
InstallDir "$DESKTOP\\MyShell"
Icon "installer.ico";

!include "MUI2.nsh"

!define MUI_ABORTWARNING
!define MUI_ICON "installer.ico"
!define MUI_TITLE "MyShell Installer"
!define MUI_WELCOMEPAGE_TITLE "Welcome to MyShell"


!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES


!insertmacro MUI_LANGUAGE "English"

Section "MainSection"
    SetOutPath "$INSTDIR"
    File "MyShell.exe"
    File "libminiz.dll"
    File "libtinyxml2.dll"
    File "archive.zip"
    File "appsettings.xml"

    SetOutPath "$INSTDIR\\commands"
    File "cal.exe"
    File "tac.exe"
    File "echo.exe"
    File "ls.exe"
    File "libminiz.dll"; 
SectionEnd
