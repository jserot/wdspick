[Setup]
AppId={{5D1BF4DD-8593-48B3-96CA-0225513733D8}
AppName=Wdspick
AppVersion=2.2
AppContact=jocelyn.serot@free.fr
AppCopyright=Copyright (C) 2018 J. Serot
AppPublisher=J. Serot
AppPublisherURL=http://www.astrosurf.com/legalet/Astro/WdsPIck.html
LicenseFile=Z:\Qt\wdspick\LICENSE
UsePreviousAppDir=false
DefaultDirName={pf}/Wdspick
DefaultGroupName=Wdspick
Compression=none
;Compression=lzma2
;SolidCompression=yes
OutputBaseFilename=Wdspick_setup
OutputDir=.
;WizardImageFile=Z:\Qt\wdspick\dist\windows\build\wdspick.bmp
;WizardSmallImageFile=Z:\Qt\qt\wdspick\dist\windows\build\wdspick_small.bmp

[Files]
Source: "Z:\Qt\wdspick\dist\windows\build\*"; DestDir: "{app}"; Flags: recursesubdirs
 
[Icons]
Name: "{group}\Wdspick"; Filename: "{app}\wdspick.exe";IconFilename: "Z:\Qt\wdspick\dist\windows\build\wdspick.ico"
Name: "{group}\{cm:UninstallProgram,Wdspick}"; Filename: "{uninstallexe}";IconFilename: "Z:\Qt\wdspick\dist\windows\build\wdspickun.ico"
Name: "{commondesktop}\Wdspick"; Filename: "{app}\Wdspick.exe"; IconFilename: "Z:\Qt\wdspick\dist\windows\build\wdspick.ico"

[Run]
Filename: "{app}\wdspick.exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

[INI]
Filename: "{app}\wdspick.ini"; Section: "Settings"; Flags: uninsdeletesection
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "wds_file"; String: "{app}\wds.txt"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "orb_file"; String: "{app}\orb.txt"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "wds_cat_index"; String: "5"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "cdc_tcp_addr"; String: "127.0.0.1"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "cdc_tcp_port"; String: "3292"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "mes_file"; String: "{app}\mesures.csv"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "log_file_prefix"; String: "wdspick_"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "launch_cdc_on_start"; String: "no"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "cdc_path"; String: "{code:GetFiles|0}"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "cdc_args"; String: ""
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "obs_long"; String: "{code:GetLoc|0}"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "obs_lat"; String: "{code:GetLoc|1}"
Filename: "{app}\wdspick.ini"; Section: "Settings"; Key: "adc_file"; String: "{app}\adc.txt"

[UninstallDelete]
Type: files; Name: "{app}\wdspick.ini"

[Code]
var
  ObsLocPage: TInputQueryWizardPage;
  FilePage: TInputFileWizardPage;
  ObsLatitude: String;
  ObsLongitude: String;
  SkyChartProgramLocation: String;

function GetFiles(Param: String): String;
begin
  Result := FilePage.Values[StrToInt(Param)];
end;

function GetLoc(Param: String): String;
begin
  Result := ObsLocPage.Values[StrToInt(Param)];
end;

procedure InitializeWizard;
begin
  { Data input page for specifying observatory location }
  ObsLocPage := CreateInputQueryPage(wpSelectComponents, 'Set location of your observatory', '', '');
  ObsLocPage.Add('Latitude ([-]xx.xxxx):', False);
  ObsLocPage.Add('Longitude ([-]xx.xxxx):', False);
  ObsLongitude := ObsLocPage.Values[0];
  ObsLatitude := ObsLocPage.Values[1];
  { File input page for specifying auxilliary programs }
  FilePage := CreateInputFilePage(wpSelectDir, 'Select Location of SkyChart (Cartes du Ciel) program', '', '');
  FilePage.Add('SkyChart:', 'Executable files|*.exe|All files|*.*', '.exe');
  FilePage.Values[0] := ExpandConstant('{pf}\SkyChart\skychart.exe'); 
  SkyChartProgramLocation := FilePage.Values[0];
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  { store chosen parameters for the next run of the setup }
  SetPreviousData(PreviousDataKey, 'CdcPath', FilePage.Values[0]);
  SetPreviousData(PreviousDataKey, 'ObsLongitude', ObsLocPage.Values[0]);
  SetPreviousData(PreviousDataKey, 'ObsLatitude', ObsLocPage.Values[1]);
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID=wpReady then
  begin
   Wizardform.ReadyMemo.Lines.Add('');
   Wizardform.ReadyMemo.Lines.Add('SkyChart Program:');
   Wizardform.ReadyMemo.Lines.Add('    ' + FilePage.Values[0]);
   Wizardform.ReadyMemo.Lines.Add('');
   Wizardform.ReadyMemo.Lines.Add('Observatory Location:');
   Wizardform.ReadyMemo.Lines.Add('    LONG=' + ObsLocPage.Values[0] + '  LAT=' + ObsLocPage.Values[1]);
 end;
end;
