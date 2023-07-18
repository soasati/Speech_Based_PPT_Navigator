Set WshShell = WScript.CreateObject("WScript.Shell")
Set objPPT = CreateObject("PowerPoint.Application")
objPPT.Visible = True

Set objPresentation = objPPT.Presentations.Open("C:\Users\Vaishali\test\ppt1.pptx")

WScript.Sleep 5000
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500
'WshShell.SendKeys "{ENTER}"