/*============================================================================*/
/* SystemIncludes.h -- Library Library Includes                                */
/*============================================================================*/
#pragma once

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <float.h>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <memory.h>
#include <netdb.h>
#include <pthread.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <typeinfo>

#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/syscall.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#include <execinfo.h>
#include <signal.h>

#ifdef WITH_APPLE
  #include <mach/thread_act.h>
  #include <libkern/OSAtomic.h>
  #include <mach/vm_map.h>
  #include <mach/mach_time.h>
  #include <mach/thread_policy.h>
  #include <mach/mach.h>
  #include <mach/message.h>
  #include <mach/kern_return.h>
  #include <mach/task_info.h>
#endif

#include <netinet/in.h>
//#include <pcap/pcap.h>
#include <arpa/inet.h>

#ifdef WITH_PYTHON_SUPPORT
  #include "/usr/include/python2.7/Python.h"
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
