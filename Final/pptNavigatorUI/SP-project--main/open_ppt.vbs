Set WshShell = WScript.CreateObject("WScript.Shell")
Set objPPT = CreateObject("PowerPoint.Application")
objPPT.Visible = True

Set objPresentation = objPPT.Presentations.Open("C:\Users\msen\Documents\Visual Studio 2010\Projects\12_PPTNavigator\pptNavigatorUI\pptNavigatorUI\SP-project--main\ppt1.pptx")

WScript.Sleep 1500
WshShell.AppActivate "SPProject.exe"
Wscript.Sleep 1500
'WshShell.SendKeys "{ENTER}"