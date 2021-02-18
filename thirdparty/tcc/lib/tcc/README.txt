The purpose of the libc.dylib file is to fix a problem introduced on MacOS 11 (Big Sur).

Big Sur introduces a cache of system libraries which don't actually have a representation
in the filesystem. So you can dlopen() a library and Big Sur will read it from its
cache, but if you go looking for the file then it logically doesn't exist.

When TinyC compiles some source code it looks for the libc.dylib file in a number of
different locations. In order to determine which location to use it first tries to 
open that file. If the file cannot be opened then it falls back to the next path to
try (and so on). However with this change in Big Sur the libc.dylib file cannot be
found in any path, so TinyC gives up before it will dlopen() it (even though dlopen()
would actually succeed). 

One of the paths that TinyC looks at is this current location. So we tried copying 
another dylib file into this path and renaming it as libc.dylib. It worked! So this
should only affect MacOS 11 Big Sur and not prior MacOS versions or Linux (at least
we hope so).

Eventually when we move on from using TinyC this file will disappear. A small dylib
file was chosen. Note that the file could not be truncated - it needed to be a valid
and complete dylib file.
