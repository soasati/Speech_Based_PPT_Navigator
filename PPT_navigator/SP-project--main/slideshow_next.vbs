Set WshShell = WScript.CreateObject("WScript.Shell")
Set Application = CreateObject("Powerpoint.application")
Application.SlideShowWindows(1).Activate
Wscript.Sleep 1500
WshShell.SendKeys "{ENTER}"

WScript.Sleep 5000
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500