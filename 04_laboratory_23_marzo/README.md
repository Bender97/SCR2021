## esercizio 1
strace: system trace. Mostra la sequenza delle system calls fatte da un programma.
*espansione del path*: tentativi che fa il programma per trovare l'eseguibile richiesto

```
	# strace ./root-ls >& log.log
	strace ./root-ls
```

execlp -> execve
	tutti gli exec sono dei wrapper per execve. Questa è l'entry point del kernel
	the 64 vars passed at the end are the environment variables

## shell creation
	execve("./root-ls", ["./root-ls", "."], 0x7ffd6dce1eb8 /* 64 vars */) = 0

## io streams: file descriptor
fcntl - manipulate file descriptor
File descriptor flags: 
	F_GETFD (void)
	              Return  (as  the function result) the file descriptor flags; arg
	              is ignored.

	fcntl(0, F_GETFD)                       = 0
	fcntl(1, F_GETFD)                       = 0
	fcntl(2, F_GETFD)                       = 0

## should the os use optimized-version of a library or not?
	access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
	access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)

/etc/ld.so.nohwcap
	When this file is present the dynamic linker will load the non-optimized version of a library, even if the CPU supports the optimized version.



## path expansion
	--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=17271, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
	execve("/usr/local/sbin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = -1 ENOENT (No such file or directory)
	execve("/usr/local/bin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = -1 ENOENT (No such file or directory)
	execve("/usr/sbin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = -1 ENOENT (No such file or directory)
	execve("/usr/bin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = -1 ENOENT (No such file or directory)
	execve("/sbin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = -1 ENOENT (No such file or directory)
	execve("/bin/ls", ["ls", "."], 0x7ffc7f005810 /* 64 vars */) = 0