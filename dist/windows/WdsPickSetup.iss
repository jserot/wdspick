[Setup]
AppId={{5D1BF4DD-8593-48B3-96CA-0225513733D8}
AppName=Wdspick
AppVersion=2.2
AppContact=jocelyn.serot@free.fr
AppCopyright=Copyright (C) 2018 J. Serot
AppPublisher=J. Serot
AppPublisherURL=http://www.astrosurf.com/legalet/Astro/WdsPIck.html
LicenseFile=E:\Spool\qt\wdspick\LICENSE
UsePreviousAppDir=false
DefaultDirName={pf}/Wdspick
DefaultGroupName=Wdspick
Compression=none
;Compression=lzma2
;SolidCompression=yes
OutputBaseFilename=Wdspick_setup
OutputDir=.
;WizardImageFile=E:\Spool\qt\wdspick\dist\windows\build\wdspick.bmp
;WizardSmallImageFile=E:/Spool/qt/wdspick/dist/windows/build/wdspick_small.bmp

[Files]
Source: "E:\Spool\qt\wdspick\dist\windows\build\*"; DestDir: "{app}"
 
[Icons]
Name: "{group}\Wdspick"; Filename: "{app}\wdspick.exe";IconFilename: "E:\Spool\qt\wdspick\dist\windows\build\wdspick.ico"
Name: "{group}\{cm:UninstallProgram,Wdspick}"; Filename: "{uninstallexe}";IconFilename: "E:\Spool\qt\wdspick\dist\windows\build\wdspickun.ico"
Name: "{commondesktop}\Wdspick"; Filename: "{app}\Wdspick.exe"; IconFilename: "E:\Spool\qt\wdspick\dist\windows\build\wdspick.ico"

[Run]
Filename: "{app}\wdspick.exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

[INI]
Filename: "{app}\wdspick.ini"; Section: "Settings"; Flags: uninsdeletesection
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "SKYCHART"; String: "{code:GetFiles|0}"

[UninstallDelete]
Type: files; Name: "{app}\wdspick.ini"

[Code]
var
  FilePage: TInputFileWizardPage;
  SkyChartProgramLocation: String;

function GetFiles(Param: String): String;
begin
  Result := FilePage.Values[StrToInt(Param)];
end;

procedure InitializeWizard;
begin
  { File input page for specifying auxilliary programs }
  FilePage := CreateInputFilePage(wpSelectDir, 'Select Location of SkyChart (Cartes du Ciel) program', '', '');
  FilePage.Add('SkyChart:', 'Executable files|*.exe|All files|*.*', '.exe');
  FilePage.Values[0] := ExpandConstant('{pf}\SkyChart\skychart.exe'); 
  SkyChartProgramLocation := FilePage.Values[0];
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  { store chosen directories for the next run of the setup }
  SetPreviousData(PreviousDataKey, 'FileLocation1', FilePage.Values[0]);
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID=wpReady then
  begin
   Wizardform.ReadyMemo.Lines.Add('');
   Wizardform.ReadyMemo.Lines.Add('SkyChart Program:');
   Wizardform.ReadyMemo.Lines.Add('    ' + FilePage.Values[0]);
 end;
end;
