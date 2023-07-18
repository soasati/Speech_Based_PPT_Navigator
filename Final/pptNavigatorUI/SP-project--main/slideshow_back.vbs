Set WshShell = WScript.CreateObject("WScript.Shell")
Set Application = CreateObject("Powerpoint.application")
Application.SlideShowWindows(1).Activate
Wscript.Sleep 1500
WshShell.SendKeys "{UP}"


WScript.Sleep 1500
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500
'WshShell.SendKeys "{ENTER}"