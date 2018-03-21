## PCCompat
A custom PCCompat.dll that enables you to hard code stats about your PC to force compatibility if required.
The original DLL uses Windows Experience Index (WEI), which is a PC assessment tool, among other things to get particular statistics about how well the local machine performs in different areas. Sometimes this software does not function properly thus this project serves as a way to force it to return particular stats to the host application.

## RenameOrdinals
When exporting the ordinal names, I was not able to decorate (aka name mangling) the names the same way as Microsoft Game Studios did. So I gave up trying and wrote a separate application that finds the export table of NT Portable Executables and enables you to rename the functions as per the input arguments. This application is automatically invoked on the post-build event for PCCompat.
