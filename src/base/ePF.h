/*============================================================================*/
/* ePF.h -- PROMPT Function                                                   */
/*============================================================================*/
#pragma once

enum ePF // prompt function
{
  /*  0 */ PF_NONE=0,
  
  /*    */ PF_ANTLR,
  /*    */ PF_APIDEMO,
  /*    */ PF_ARGS,
  /*    */ PF_ASCII,
  /*    */ PF_ATTACHMAP,
  
  /*    */ PF_BG,
  /*    */ PF_BP,
  /*    */ PF_BPA,
  /*    */ PF_BYE,
  
  /*    */ PF_CALC,
  /*    */ PF_CANCEL,
  /*    */ PF_CD,
  /*    */ PF_CLASSES,
  /*    */ PF_CMDS,
  /*    */ PF_CONFIG,
  /*    */ PF_CONSTANTS,
  
  /*    */ PF_DDLLOCK,
  /*    */ PF_DELTA,
  /*    */ PF_DELTARESET,
  /*    */ PF_DESCRIBE,
  /*    */ PF_DICTREE,
  /*    */ PF_DSV,
  /*    */ PF_DUMPTOKENS,

  /*    */ PF_EDIT,
  /*    */ PF_ENUMS,
  /*    */ PF_ERRNO,
  /*    */ PF_EXECUTING,
  
  /*    */ PF_FACT,
  /*    */ PF_FILECAT,
  /*    */ PF_FILEPROC,
  /*    */ PF_FILERAND,
  /*    */ PF_FILESTATS,
  /*    */ PF_FLOATS,
  /*    */ PF_FOLDERS,
  /*    */ PF_FUNS,
  
  /*    */ PF_GIVEMEM,
  /*    */ PF_GLOBALS,
  
  /*    */ PF_HASHDEFS,
  /*    */ PF_HEAPSUMMARY,
  /*    */ PF_HELP,
  /*    */ PF_HEXDUMP,
  /*    */ PF_HOW,
  /*    */ PF_HW,
  
  /*    */ PF_INDEXENTRIES,
  
  /*    */ PF_JOB,
  /*    */ PF_JOBCLASS,
  /*    */ PF_JOBS,

  /*    */ PF_LASTJOB,
  /*    */ PF_LET,
  /*    */ PF_LIMITS,
  /*    */ PF_LOG,
  /*    */ PF_LOGNOTE,
  /*    */ PF_LOGSECTION,
  /*    */ PF_LOGWRITE,
  /*    */ PF_LS,
  /*    */ PF_LVL,
  
  /*    */ PF_MASKS,
  /*    */ PF_MEMORY,
  /*    */ PF_MEMORYLEAKS,
  /*    */ PF_MEMTEST,
  /*    */ PF_MKFIFO,
  /*    */ PF_MON,
  /*    */ PF_MRI,
  
  /*    */ PF_NOW,
  /*    */ PF_NUMBERS,
  
  /*    */ PF_OBJECTLIST,
  /*    */ PF_OBJECTVALIDATE,
  /*    */ PF_OLIST,
  /*    */ PF_OPTIMISERLVL,
  /*    */ PF_OUTPUT,
  /*    */ PF_OPCAT,
  /*    */ PF_OPS,
  /*    */ PF_OPSYS,

  /*    */ PF_PDSTEST,
  /*    */ PF_PLIST,
  /*    */ PF_PMC,
  /*    */ PF_POW2,
  /*    */ PF_PREFIXES,
  /*    */ PF_PRINT,
  /*    */ PF_PROCEDURE,
  /*    */ PF_PROMPT,
  /*    */ PF_PROMPTDEBUG,
  /*    */ PF_PWD,
  
  /*    */ PF_QUESTION,
  
  /*    */ PF_RASSERT,
  /*    */ PF_READY,
  /*    */ PF_RESOURCES,
  /*    */ PF_RETIRED,
  /*    */ PF_RMFIFO,
  /*    */ PF_RND,
  
  /*    */ PF_SCAN,
  /*    */ PF_SE,
  /*    */ PF_SELECT,
  /*    */ PF_SELFIES,
  /*    */ PF_SETDBG,
  /*    */ PF_SHELL,
  /*    */ PF_SHORTCUT,
  /*    */ PF_SHOW,
  /*    */ PF_SINCE,
  /*    */ PF_SIZES,
  /*    */ PF_SLEEP,
  /*    */ PF_SLEEPERS,
  /*    */ PF_SLEEPING,
  /*    */ PF_SNAPSHOTDELTA,
  /*    */ PF_SNAPSHOTINSPECT,
  /*    */ PF_SNAPSHOTLIST,
  /*    */ PF_SNAPSHOTNEWER,
  /*    */ PF_SNAPSHOTOBJECTS,
  /*    */ PF_SO,
  /*    */ PF_SOCKET,
  /*    */ PF_ST,
  /*    */ PF_START,
  /*    */ PF_STARTCOMPLETED,
  /*    */ PF_STARTLINE,
  /*    */ PF_STARTREADY,
  /*    */ PF_STARTRESET,
  /*    */ PF_STARTSTATUS,
  /*    */ PF_STORE,
  
  /*    */ PF_TAG,
  /*    */ PF_TAGS,
  /*    */ PF_TAKEMEM,
  /*    */ PF_TASKQUEUES,
  /*    */ PF_TEMPNAME,
  /*    */ PF_THREADS,
  /*    */ PF_TIMER,
  /*    */ PF_TOOLS,
  /*    */ PF_TPCH,
  /*    */ PF_TRACKMEMORY,

  /*    */ PF_UPTIME,
  /*    */ PF_UT_SKIPLIST,

  /*    */ PF_VERBOSE,
  /*    */ PF_VERSION,
  
  /*    */ PF_WAIT_BG,
  /*    */ PF_WAITING,
  /*    */ PF_WITH,
  /*    */ PF_WITHOUT,

  /*    */ PF_ENUMT_
};

extern Stz* aPF[PF_ENUMT_];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
