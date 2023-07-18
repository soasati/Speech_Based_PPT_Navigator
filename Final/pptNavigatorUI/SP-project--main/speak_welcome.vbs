Dim msg, sapi
Set sapi = createObject("sapi.spvoice")
Set sapi.Voice = sapi.GetVoices.Item(1)
sapi.Speak "Welcome to PPT Navigator"
'Lol = msgbox("Request done.")