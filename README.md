# symbol_extractor tool

lets you disassemble and extract functions at scale off windows librarys.

## usage:

.\extractor.exe kernel32.dll srv*C:\symbols*https://msdl.microsoft.com/download/symbols KERNEL32!CreateProcessW .
srv*C:\symbols*https://chromium-browser-symsrv.commondatastorage.googleapis.com