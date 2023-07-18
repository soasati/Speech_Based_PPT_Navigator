Dim msg, sapi
Set sapi = createObject("sapi.spvoice")
Set sapi.Voice = sapi.GetVoices.Item(1)
sapi.Speak "Please speak to continue"
'Lol = msgbox("Request done.")