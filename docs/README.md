## DLL hijacking
### DOpus.exe
Importer | Thread | DLL
--- | --- | ---
| dopus.exe | A | USERENV.dll
| dopus.exe, dopuslib.dll | B | VERSION.dll
| dopus.exe | A | WININET.dll
| dopus.exe | B | MPR.dll
| dopus.exe | C | WINMM.dll
| dopus.exe | B | MSVFW32.dll
| dopus.exe | A | MSACM32.dll
| dopus.exe | D | urlmon.dll
| dopus.exe | C | NETAPI32.dll
| dopus.exe, dopuslib.dll | B | UxTheme.dll
| dopus.exe | C | query.dll
| dopus.exe, dopuslib.dll | D | WTSAPI32.dll
| | C | iertutil.dll
| | D | srvcli.dll
| | B | netutils.dll
| | D | SHFOLDER.dll
| | A | WKSCLI.DLL
| | C | SAMCLI.DLL
| | D | profapi.dll
| | D | ncrypt.dll
| | C | BCRYPT.DLL
| | D | NTASN1.dll
| | D | MSASN1.dll
| | D | DSROLE.DLL
| | D | WindowsCodecs.dll
| | C | USP10.dll
| | A | msls31.dll
| | D | CoreMessaging.dll
| | D | CoreUIComponents.dll
| | D | CRYPTBASE.DLL
| | D | OLEACCRC.DLL
| | E | CFGMGR32.dll
| | F | `/dopusdata\Images\shell32.dll`
| | F | `Images\shell32.dll`
| | F | shell32.dll

### DOpusRT.exe
- SHFOLDER.dll
- version.dll
- UxTheme.dll
- WTSAPI32.dll