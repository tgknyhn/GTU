#ifndef DAEMON_H
#define DAEMON_H

// Daemon define values
#define BD_NO_CHDIR 01              /* Don't chdir */
#define BD_NO_CLOSE_FILES 02        /* Don't close open files */
#define BD_NO_REOPEN_STD_FDS 04     /* Don't reopen standart file descriptors with "/dev/null" */
#define BD_NO_UMASK0 010            /* Don't do umask(0) */     
#define BD_MAX_CLOSE 8192           /* Max fd to close when sysconf(_SC_OPEN_MAX) is indeterminate */    

int becomeDaemon(int flags);

#endif