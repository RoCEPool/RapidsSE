/*============================================================================*/
/* PromptFun.hpp -- a description of a prompt function                        */
/*============================================================================*/
#pragma once

namespace Prompt
{
  enum ePF // prompt function
  {
  /*  0 */ PF_NONE=0,
  
  /*    */ PF_ACTIONSEQUENCE,
  /*    */ PF_ACTIVITYMAP,
  /*    */ PF_AM,
  /*    */ PF_AMSECTION,
  /*    */ PF_APISTATE,
  /*    */ PF_ARGS,
  /*    */ PF_ASCII,
  /*    */ PF_ATTACHMAP,
  /*    */ PF_BP,
  /*    */ PF_BYE,
  /*    */ PF_CACHE,
  /*    */ PF_CD,
  /*    */ PF_CLASSES,
  /*    */ PF_CMDS,
  /*    */ PF_CONFIG,
  /*    */ PF_CONSTANTS,
  /*    */ PF_DATE,
  /*    */ PF_ENUMS,
  /*    */ PF_ERRNO,
  /*    */ PF_EXECUTING,
  /*    */ PF_FILECAT,
  /*    */ PF_FILEPROC,
  /*    */ PF_FILESTATS,
  /*    */ PF_FLOATS,
  /*    */ PF_FUNS,
  /*    */ PF_GIVEMEM,
  /*    */ PF_HASHDEFS,
  /*    */ PF_HEAPMEM,
  /*    */ PF_HELP,
  /*    */ PF_HEXDUMP,
  /*    */ PF_HW,
  /*    */ PF_INDEXENTRIES,
  /*    */ PF_JOB,
  /*    */ PF_JOBCAT,
  /*    */ PF_JOBCLASS,
  /*    */ PF_LOCKS,
  /*    */ PF_LS,
  /*    */ PF_LVL,
  /*    */ PF_MARKACT,
  /*    */ PF_MARKAPI,
  /*    */ PF_MARKJOB,
  /*    */ PF_MARKRES,
  /*    */ PF_MARKTRC,
  /*    */ PF_MASKS,
  /*    */ PF_MEMORY,
  /*    */ PF_MEMTEST,
  /*    */ PF_MKPIPE,
  /*    */ PF_MON,
  /*    */ PF_NUMBERS,
  /*    */ PF_OLIST,
  /*    */ PF_OPSYS,
  /*    */ PF_PLIST,
  /*    */ PF_PMC,
  /*    */ PF_POW2,
  /*    */ PF_PREFIXES,
  /*    */ PF_PRINT,
  /*    */ PF_PWD,
  /*    */ PF_QUESTION,
  /*    */ PF_RASSERT,
  /*    */ PF_READY,
  /*    */ PF_RESOURCES,
  /*    */ PF_RMPIPE,
  /*    */ PF_RND,
  /*    */ PF_SE,
  /*    */ PF_SHELL,
  /*    */ PF_SINCE,
  /*    */ PF_SIZES,
  /*    */ PF_SLEEP,
  /*    */ PF_SLEEPERS,
  /*    */ PF_SLEEPING,
  /*    */ PF_SOCKET,
  /*    */ PF_SSN,
  /*    */ PF_ST,
  /*    */ PF_TAKEMEM,
  /*    */ PF_TASKQUEUES,
  /*    */ PF_TEMPNAME,
  /*    */ PF_THREADS,
  /*    */ PF_TIMEREND,
  /*    */ PF_TIMERSTART,
  /*    */ PF_TPCH,
  /*    */ PF_UPTIME,
  /*    */ PF_VERSION,
  /*    */ PF_WAITING,
  
  /*    */ PF_ENUMS_
  };
  
  static Stz* aPF[]
  {
  /*  0 */ "NONE"_Stz,
  /*    */ "ACTIONSEQUENCE"_Stz,
  /*    */ "ACTIVITYMAP"_Stz,
  /*    */ "AM"_Stz,
  /*    */ "AMSECTION"_Stz,
  /*    */ "APISTATE"_Stz,
  /*    */ "ARGS"_Stz,
  /*    */ "ASCII"_Stz,
  /*    */ "ATTACHMAP"_Stz,
  /*    */ "BP"_Stz,
  /*    */ "BYE"_Stz,
  /*    */ "CACHE"_Stz,
  /*    */ "CD"_Stz,
  /*    */ "CLASSES"_Stz,
  /*    */ "CMDS"_Stz,
  /*    */ "CONFIG"_Stz,
  /*    */ "CONSTANTS"_Stz,
  /*    */ "DATE"_Stz,
  /*    */ "ENUMS"_Stz,
  /*    */ "ERRNO"_Stz,
  /*    */ "EXECUTING"_Stz,
  /*    */ "FILECAT"_Stz,
  /*    */ "FILEPROC"_Stz,
  /*    */ "FILESTATS"_Stz,
  /*    */ "FLOATS"_Stz,
  /*    */ "FUNS"_Stz,
  /*    */ "GIVEMEM"_Stz,
  /*    */ "HASHDEFS"_Stz,
  /*    */ "HEAPMEM"_Stz,
  /*    */ "HELP"_Stz,
  /*    */ "HEXDUMP"_Stz,
  /*    */ "HW"_Stz,
  /*    */ "INDEXENTRIES"_Stz,
  /*    */ "JOB"_Stz,
  /*    */ "JOBCAT"_Stz,
  /*    */ "JOBCLASS"_Stz,
  /*    */ "LOCKS"_Stz,
  /*    */ "LS"_Stz,
  /*    */ "LVL"_Stz,
  /*    */ "MARKACT"_Stz,
  /*    */ "MARKAPI"_Stz,
  /*    */ "MARKJOB"_Stz,
  /*    */ "MARKRES"_Stz,
  /*    */ "MARKTRC"_Stz,
  /*    */ "MASKS"_Stz,
  /*    */ "MEMORY"_Stz,
  /*    */ "MEMTEST"_Stz,
  /*    */ "MKPIPE"_Stz,
  /*    */ "MON"_Stz,
  /*    */ "NUMBERS"_Stz,
  /*    */ "OLIST"_Stz,
  /*    */ "OPSYS"_Stz,
  /*    */ "PLIST"_Stz,
  /*    */ "PMC"_Stz,
  /*    */ "POW2"_Stz,
  /*    */ "PREFIXES"_Stz,
  /*    */ "PRINT"_Stz,
  /*    */ "PWD"_Stz,
  /*    */ "QUESTION"_Stz,
  /*    */ "RASSERT"_Stz,
  /*    */ "READY"_Stz,
  /*    */ "RESOURCES"_Stz,
  /*    */ "RMPIPE"_Stz,
  /*    */ "RND"_Stz,
  /*    */ "SE"_Stz,
  /*    */ "SHELL"_Stz,
  /*    */ "SINCE"_Stz,
  /*    */ "SIZES"_Stz,
  /*    */ "SLEEP"_Stz,
  /*    */ "SLEEPERS"_Stz,
  /*    */ "SLEEPING"_Stz,
  /*    */ "SOCKET"_Stz,
  /*    */ "SSN"_Stz,
  /*    */ "ST"_Stz,
  /*    */ "TAKEMEM"_Stz,
  /*    */ "TASKQUEUES"_Stz,
  /*    */ "TEMPNAME"_Stz,
  /*    */ "THREADS"_Stz,
  /*    */ "TIMEREND"_Stz,
  /*    */ "TIMERSTART"_Stz,
  /*    */ "TPCH"_Stz,
  /*    */ "UPTIME"_Stz,
  /*    */ "VERSION"_Stz,
  /*    */ "WAITING"_Stz,

  /*    */ "ENUMS_"_Stz
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptFun
  {
  public:
    ePF  m_ePromptFun;   // prompt function
    Stz* m_aName;        // name of the prompt function
    Stz* m_aComment;     // comment about the function
    Stz* m_aCommand1;    // first word of prompt command
    Stz* m_aCommand2;    // optional second word of prompt command
    bool (*m_fPrompt)(SePrompt* pPrompt);
    bool m_bEnabled;     // is this function enabled
    bool m_bVisible;     // is this function visible
    Stz* m_aHelp;        // help about the function
    Stz* m_aDesc;        // description

  public:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
