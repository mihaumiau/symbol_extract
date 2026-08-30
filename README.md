# symbol_extractor tool

lets you disassemble and extract functions at scale off windows librarys.

## usage:

````markdown
.\extractor.exe <library_dir> <library_name> <symbol_location> [<symbol_name>...] <disassemble_out>
.\extractor.exe . kernel32.dll srv*C:\symbols*https://msdl.microsoft.com/download/symbols KERNEL32!CreateProcessW .
.\extractor.exe 'C:\Program Files\Google\Chrome\Application\152.0.7977.65' chrome.dll https://chromium-browser-symsrv.commondatastorage.googleapis.com 'chrome!`anonymous namespace''::TCPServerSocketFactory::CreateForHttpServer' CreateForHttpServer.txt chrome!malloc malloc.txt chrome!content::DevToolsAgentHost::StartRemoteDebuggingServer StartRemoteDebuggingServer.txt C:\Users\user\Desktop\dbg-test\chrome
````
