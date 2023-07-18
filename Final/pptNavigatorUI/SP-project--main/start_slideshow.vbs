Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.AppActivate "Powerpoint"
Set Powerpointapp = CreateObject("Powerpoint.application")
Powerpointapp.Visible = True
Powerpointapp.Activate
WshShell.SendKeys "{F5}"
WScript.Sleep 1000
WshShell.SendKeys "^L"

WScript.Sleep 10000
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500