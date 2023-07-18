Dim msg, sapi
Set sapi = createObject("sapi.spvoice")
Set sapi.Voice = sapi.GetVoices.Item(1)
sapi.Speak "Sorry word is not recognized. please try again."
'Lol = msgbox("Request done.")