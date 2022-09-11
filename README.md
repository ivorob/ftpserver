# Simple ftp server

[![ftpserver](https://github.com/ivorob/ftpserver/actions/workflows/cmake.yml/badge.svg)](https://github.com/ivorob/ftpserver/actions)

## What is it and what does it do?

It is a simple ftp server written in C++.
It comes without any warranties of any kind.

## How to use?

### Building

To build run the following commands:

```
mkdir build
cd build
cmake -G "Unix Makefiles" ..
cmake --build .
```

If all went well, an executable file named `ftpserver` should have been generated.

### Run the server

You can execute the executable generated in the compilation step by `./ftpserver`.

Command-line options are as follows:

ftpserver `server root directory, default=./` `port to listen on for incoming connections, default=4242` `telnet mode for use with telnet client, default=false`

To shut down, press Ctrl+C.

While it is running, a ftp client can connect to it.

### Usage reference

Available commands in the program that the client can issue and the running ftpserver will respond to:

| Command | Explanation |
| --- | --- |
| user `username` | login as user `username`, dummy implemented |
| syst | get system type without system version |
| browse | lists all files and directories in the current server working directory |
| ls `dir` | lists all files and directories in the specified directory |
| download `file` | sends the binary data of the specified file over the connection the command was received |
| upload `file` | stores all binary data that is received by the connection the command was received |
| pwd | prints the current working directory on the server |
| cwd `dir` | changes the current working directory on the server to the specified one |
| touch `file` | create an empty file of the given name |
| mkdir `dir` | creates a directory of the specified name in the current server working dir |
| rmdir `dir` | removes the specified directory and all contents at the server side |
| delete `file` | deletes the specified file at the server side |
| getparentdir | returns the parent directory of the current working dir |
| getsize `file` | returns the size in bytes of the specified file |
| getsize `dir` | returns the content count, the number of subdirectories and files in the specified directory |
| getaccessright `file or dir` | returns the numeric unix permission for a specified file or directory |
| getlastmodificationtime `file or dir` | returns the time of the last modification for a specified file or directory |
| getowner `file or dir` | returns the file or directory owner |
| getgroup `file or dir` | returns the file or directory group |
| bye | terminates the connection the command is received on |
| quit | same as disconnect |

## TODO

* Align to standard FTP *SERVER* commands, see http://www.nsftools.com/tips/RawFTP.htm and https://www.rfc-editor.org/rfc/rfc959
* Refactor the code to align to new C++ versions and standards
* Refactor the code to make standard ftp clients work
* Refactor the code to add Windows support
