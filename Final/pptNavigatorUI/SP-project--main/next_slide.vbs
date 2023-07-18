Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.AppActivate "Powerpoint"
Set Powerpointapp = CreateObject("Powerpoint.application")
Powerpointapp.Visible = True
Powerpointapp.Activate
WshShell.SendKeys "{DOWN}"

WScript.Sleep 1500
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500
'WshShell.SendKeys "{ENTER}"