Dim msg, sapi
Set sapi = createObject("sapi.spvoice")
Set sapi.Voice = sapi.GetVoices.Item(1)
sapi.Speak "Word goto is recognised"
'Lol = msgbox("Request done.")