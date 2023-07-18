Dim msg, sapi
Set sapi = createObject("sapi.spvoice")
Set sapi.Voice = sapi.GetVoices.Item(1)
sapi.Speak "word four is recognized"
'Lol = msgbox("Request done.")