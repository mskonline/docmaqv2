;Setup Program for DocmaQ

[Setup]
AppName=DocmaQ 2.1
AppVersion=2.1
AppVerName=DocmaQ 2.1
AppCopyright=Copyright (C) 2014
AllowRootDirectory=no
Compression=lzma2
CloseApplications=yes
DefaultDirName={pf}\DocmaQ
DefaultGroupName=DocmaQ
InfoAfterFile=README.txt
PrivilegesRequired=lowest
SetupIconFile=print.ico
SetupLogging=Yes
ShowTasksTreeLines=yes
ShowLanguageDialog=no
TimeStampsInUTC=yes
UninstallDisplayIcon={app}\DocmaQ.exe
UninstallDisplayName=DocmaQ
UsePreviousAppDir=yes
WizardImageFile=installpic.bmp
 
[Files]
Source: "../src/release/DocmaQ.exe"; DestDir: "{app}"
Source: "DocmaQ_Configure.exe"; DestDir: "{app}"
Source: "QtCore4.dll"; DestDir: "{app}"
Source: "QtGui4.dll"; DestDir: "{app}"
Source: "QtNetwork4.dll"; DestDir: "{app}"
Source: "QtXml4.dll"; DestDir: "{app}"
Source: "mingwm10.dll"; DestDir: "{app}"
Source: "libmySQL.dll"; DestDir: "{app}"
Source: "libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "./plugins/sqldrivers/qsqlmysql4.dll"; DestDir: "{app}/plugins/sqldrivers"
Source: "Configurations.ini"; DestDir: "{app}"; Flags: uninsneveruninstall
Source: "installpic.bmp"; DestDir: "{app}"
Source: "gpl.html"; DestDir: "{app}"
Source: "COPYING.txt"; DestDir: "{app}"
Source: "AUTHORS.txt"; DestDir: "{app}"
Source: "release notes.txt"; DestDir: "{app}"
Source: "require.txt"; DestDir: "{app}"
Source: "src.zip"; DestDir: "{app}"
Source: "THANKS.txt"; DestDir: "{app}"
Source: "README.txt"; DestDir: "{app}"

;Fonts
Source: "./fonts/Bitstream Vera Serif.ttf"; DestDir:"{app}/fonts/"
Source: "./fonts/ERASDEMI.TTF"; DestDir:"{app}/fonts/"
Source: "./fonts/micross.ttf"; DestDir:"{app}/fonts/"
Source: "./fonts/SEGOEUI.TTF"; DestDir:"{app}/fonts/"
Source: "./fonts/verdana.ttf"; DestDir:"{app}/fonts/"

;Fonts Install
Source: "./fonts/Bitstream Vera Serif.ttf"; DestDir:"{fonts}" ;FontInstall: "Bitstream Vera Serif";Flags: onlyifdoesntexist uninsneveruninstall
Source: "./fonts/ERASDEMI.TTF"; DestDir:"{fonts}" ;FontInstall: "Eras Demi ITC";Flags: onlyifdoesntexist uninsneveruninstall
Source: "./fonts/micross.ttf"; DestDir:"{fonts}" ;FontInstall: "Microsoft Sans Serif";Flags: onlyifdoesntexist uninsneveruninstall
Source: "./fonts/SEGOEUI.TTF"; DestDir:"{fonts}" ;FontInstall: "Segoe UI";Flags: onlyifdoesntexist uninsneveruninstall
Source: "./fonts/verdana.ttf"; DestDir:"{fonts}" ;FontInstall: "Verdana";Flags: onlyifdoesntexist uninsneveruninstall

;Images
Source: "./images/bonafide.png"; DestDir:"{app}/images/"
Source: "./images/conduct.png"; DestDir:"{app}/images/"
Source: "./images/docmaq.png"; DestDir:"{app}/images/"
Source: "./images/intro.png"; DestDir:"{app}/images/"
Source: "./images/session.png"; DestDir:"{app}/images/"
Source: "./images/stpic.png"; DestDir:"{app}/images/"
Source: "./images/tc.png"; DestDir:"{app}/images/"

[Dirs]
Name: "{app}\logs"; Flags: uninsneveruninstall
Name: "{app}\logs\certificate"; Flags: uninsneveruninstall
Name: "{app}\logs\session"; Flags: uninsneveruninstall

[Icons]
Name: "{group}\DocmaQ"; Filename: "{app}\DocmaQ.exe"; WorkingDir: "{app}"
Name: "{group}\Configure"; Filename: "{app}\DocmaQ_Configure.exe"; WorkingDir: "{app}"
Name: "{group}\Read Me"; Filename: "{app}\README.txt"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\DocmaQ_Configure.exe"; Flags: nowait
