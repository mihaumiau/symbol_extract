# symbol_extractor tool

lets you disassemble and extract functions off windows librarys at scale.

## usage:
````markdown
.\extractor.exe <module_path> <symbol_location> [<symbol_name>...] <disassemble_out>
.\extractor.exe kernel32.dll srv*C:\symbols*https://msdl.microsoft.com/download/symbols KERNEL32!CreateProcessW .
````
