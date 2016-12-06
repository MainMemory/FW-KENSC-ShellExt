; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{2D45BECF-09BF-42A8-9ED2-1BFC6B197F07}
AppName=FW-KENSC
AppVersion=1.0
;AppVerName=FW-KENSC 1.0
AppPublisher=Sonic Retro
AppPublisherURL=http://sonicretro.org/
AppSupportURL=http://sonicretro.org/
AppUpdatesURL=http://sonicretro.org/
DefaultDirName={pf}\FW-KENSC
AllowNoIcons=yes
OutputBaseFilename=FW-KENSC-setup-1.0
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Check: Is64BitInstallMode; Source: "x64\Release\FW-KENSC-ShellExt.dll"; DestDir: "{app}"; DestName: "FW-KENSC-ShellExt64.dll"; Flags: ignoreversion
Check: Is64BitInstallMode; Source: "x64\Release\FW-KENSC-ShellExt.pdb"; DestDir: "{app}"; DestName: "FW-KENSC-ShellExt64.pdb"; Flags: ignoreversion
Source: "Release\FW-KENSC-ShellExt.dll"; DestDir: "{app}"; DestName: "FW-KENSC-ShellExt.dll"; Flags: ignoreversion
Source: "Release\FW-KENSC-ShellExt.pdb"; DestDir: "{app}"; DestName: "FW-KENSC-ShellExt.pdb"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKCR; Subkey: "*\shellex\ContextMenuHandlers\FW-KENSC"; ValueType: string; ValueData: "{{76F76F87-9DA0-48D6-9277-238FCEC133CA}"; Flags: uninsdeletekey
Root: HKCR32; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}"; ValueType: string; ValueData: "FW-KENSC"; Flags: uninsdeletekey
Root: HKCR32; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}\InProcServer32"; ValueType: string; ValueData: "{app}\FW-KENSC-ShellExt.dll"
Root: HKCR32; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}\InProcServer32"; ValueType: string; ValueName: "ThreadingModel"; ValueData: "Apartment"
Check: Is64BitInstallMode; Root: HKCR64; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}"; ValueType: string; ValueData: "FW-KENSC"; Flags: uninsdeletekey
Check: Is64BitInstallMode; Root: HKCR64; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}\InProcServer32"; ValueType: string; ValueData: "{app}\FW-KENSC-ShellExt64.dll"
Check: Is64BitInstallMode; Root: HKCR64; Subkey: "CLSID\{{76F76F87-9DA0-48D6-9277-238FCEC133CA}\InProcServer32"; ValueType: string; ValueName: "ThreadingModel"; ValueData: "Apartment"
