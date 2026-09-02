# symbol_extractor tool

lets you disassemble and extract functions off windows librarys at scale.

## usage:

````markdown
.\extractor.exe <library_dir> <library_name> <symbol_path> [<function_name> <disassembly_out>]... <out_dir>
.\extractor.exe . kernel32.dll SRV\;C:\symbols\;https://msdl.microsoft.com/download/symbols KERNEL32!CreateProcessW CreateProcessW.txt .
.\extractor.exe 'C:\Program Files\Google\Chrome\Application\152.0.7977.65' chrome.dll SRV\;C:\symbols\;https://chromium-browser-symsrv.commondatastorage.googleapis.com 'chrome!`anonymous namespace''::TCPServerSocketFactory::CreateForHttpServer' CreateForHttpServer.txt chrome!malloc malloc.txt chrome!content::DevToolsAgentHost::StartRemoteDebuggingServer StartRemoteDebuggingServer.txt C:\Users\user\Desktop\dbg-test\chrome
````

(currently under maintance because the dbgeng api documentation does not work like it should...)
