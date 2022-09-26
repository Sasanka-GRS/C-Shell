Run the Shell:

1. Run the command 'make' followed by ./a.out
2. A prompt will be displayed.
3. Enter the commands into the shell and hit ENTER.
4. to exit from the shell, use CTRL+C.


Features:


1. cd:

Syntax: cd [path]

Description: 
Change the current working directory to the directory specified in the path.
Error is displayed if no such path exists.
Can handle arguments [.], [..], [-] and [~].
Current working directory is displayed in the prompt.
Displays the absolute path of the shell and goes to the parent when the present working directory is home (wrt shell) and path entered is [..].

Files: cd.c, cd.h 



2. pwd:

Syntax: pwd

Description: 
Prints the present working directory with respect to the shell.

Files: pwd.c, pwd.h



3. echo:

Syntax: echo [arguments]

Description: 
Prints the arguments.
Multiline strings, environment variables, and quotes are not handeled.

Files: echo.c, echo.h



4. ls:

Syntax: ls [arguments]

Description: 
Lists the directory contents of the path specified in arguments.
If the arguments is NULL, then directory contents of present working directory are displayed
If the path does not exist error is displayed
Can handle flags [-l], [-a], [-al]/[-la]

Files: ls.c, ls.h



5. system process:

Syntax: <command> [arguments]

Descritpion:
Runs the basic system process.
Can handle flag [&] for background processes
If process does not exist error is displayed
Executed using execvp() command

Files: execute.c, execute.h



6. pinfo:

Syntax: pinfo [pid]

Descritpion:
Displays pid, process status, memory and executable path of the process with pid entered.
If pid is NULL the details of process related to shell is displayed.
If no process exists with the pid given, error is displayed.

Files: pinfo.c, pinfo.h



7. finished background process

Syntax: No syntax

Description:
Whenever a background process exits, its exit details are diaplyed.

Files: background.c, background.h 



8. history

Syntax: history [arguments]

Description:
Displays the last [argumnets] number of commands entered into the shell.
At max, only last 20 commands can be traced.
If [arguments] is NULL, last few commands, at max 10, are displayed.
If the file 'history.txt', is not created or is unable to access error is displayed.

Files: history.c, history.h

Other files:

headers.h: Contains all the headers and global variables of imported C libraries
.history.txt: Stores history of commands entered into the shell
makefile: Contains compilation command of shell
next.c: Prints the prompt for the shell with the current working directory as well as delay imposed by foreground processes
next.h: Header file for prompt
README.txt: displays basic information about shell
shell.c: contains main loop
a.out: executable of the shell