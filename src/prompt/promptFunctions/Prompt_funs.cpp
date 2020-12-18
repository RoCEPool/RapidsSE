/*============================================================================*/
/* Prompt_funs.cpp -- prompt funs function                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern PromptCatalog gxPromptCatalog;
extern PromptFun     __Prompt_actionSequence;

PromptFun __Prompt_funs (
  PF_FUNS,               // prompt function
  "Prompt_funs"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "funs"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_funs,          // prompt function
  true,                  // enabled

  true,                  // visible
  "funs"_Stz,            // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_funs(SePrompt* pPrompt)
{
  if (AM::bMetadata()) {
    vTrace("metadata: (function VARCHAR)\n"_Stz);
  }
  
  gxPromptCatalog.vFuns(PF_LOGSECTION);
  gxPromptCatalog.vFuns(PF_SSNSTATE);
  gxPromptCatalog.vFuns(PF_ARGS);
  gxPromptCatalog.vFuns(PF_ASCII);
  gxPromptCatalog.vFuns(PF_ATTACHMAP);
  gxPromptCatalog.vFuns(PF_BP);
  gxPromptCatalog.vFuns(PF_BYE);
  gxPromptCatalog.vFuns(PF_CACHE);
  gxPromptCatalog.vFuns(PF_CD);
  gxPromptCatalog.vFuns(PF_CLASSES);
  gxPromptCatalog.vFuns(PF_CMDS);
  gxPromptCatalog.vFuns(PF_CONFIG);
  gxPromptCatalog.vFuns(PF_CONSTANTS);
  gxPromptCatalog.vFuns(PF_ENUMS);
  gxPromptCatalog.vFuns(PF_ERRNO);
  gxPromptCatalog.vFuns(PF_EXECUTING);
  gxPromptCatalog.vFuns(PF_FILESTATS);
  gxPromptCatalog.vFuns(PF_FLOATS);
  gxPromptCatalog.vFuns(PF_FUNS);
  gxPromptCatalog.vFuns(PF_GIVEMEM);
  gxPromptCatalog.vFuns(PF_HASHDEFS);
  gxPromptCatalog.vFuns(PF_HEAPMEM);
  gxPromptCatalog.vFuns(PF_HELP);
  gxPromptCatalog.vFuns(PF_HEXDUMP);
  gxPromptCatalog.vFuns(PF_HW);
  gxPromptCatalog.vFuns(PF_INDEXENTRIES);
  gxPromptCatalog.vFuns(PF_JOB);
  gxPromptCatalog.vFuns(PF_JOBCLASS);
  gxPromptCatalog.vFuns(PF_LOCKS);
  gxPromptCatalog.vFuns(PF_LOGNOTE);
  gxPromptCatalog.vFuns(PF_LOGSECTION);
  gxPromptCatalog.vFuns(PF_LOGWRITE);
  gxPromptCatalog.vFuns(PF_LS);
  gxPromptCatalog.vFuns(PF_LVL);
  gxPromptCatalog.vFuns(PF_MASKS);
  gxPromptCatalog.vFuns(PF_MEMORY);
  gxPromptCatalog.vFuns(PF_MEMORYLEAKS);
  gxPromptCatalog.vFuns(PF_MEMTEST);
  gxPromptCatalog.vFuns(PF_MKFIFO);
  gxPromptCatalog.vFuns(PF_MON);
  gxPromptCatalog.vFuns(PF_NOW);
  gxPromptCatalog.vFuns(PF_NUMBERS);
  gxPromptCatalog.vFuns(PF_OLIST);
  gxPromptCatalog.vFuns(PF_OPSYS);
  gxPromptCatalog.vFuns(PF_PLIST);
  gxPromptCatalog.vFuns(PF_PMC);
  gxPromptCatalog.vFuns(PF_POW2);
  gxPromptCatalog.vFuns(PF_PREFIXES);
  gxPromptCatalog.vFuns(PF_PRINT);
  gxPromptCatalog.vFuns(PF_PWD);
  gxPromptCatalog.vFuns(PF_QUESTION);
  gxPromptCatalog.vFuns(PF_RASSERT);
  gxPromptCatalog.vFuns(PF_READY);
  gxPromptCatalog.vFuns(PF_RESOURCES);
  gxPromptCatalog.vFuns(PF_RMFIFO);
  gxPromptCatalog.vFuns(PF_RND);
  gxPromptCatalog.vFuns(PF_SE);
  gxPromptCatalog.vFuns(PF_SHELL);
  gxPromptCatalog.vFuns(PF_SHORTCUT);
  gxPromptCatalog.vFuns(PF_SINCE);
  gxPromptCatalog.vFuns(PF_SIZES);
  gxPromptCatalog.vFuns(PF_SLEEP);
  gxPromptCatalog.vFuns(PF_SLEEPERS);
  gxPromptCatalog.vFuns(PF_SLEEPING);
  gxPromptCatalog.vFuns(PF_SOCKET);
  gxPromptCatalog.vFuns(PF_SSN);
  gxPromptCatalog.vFuns(PF_ST);
  gxPromptCatalog.vFuns(PF_TAKEMEM);
  gxPromptCatalog.vFuns(PF_TASKQUEUES);
  gxPromptCatalog.vFuns(PF_TEMPNAME);
  gxPromptCatalog.vFuns(PF_THREADS);
  gxPromptCatalog.vFuns(PF_TIMEREND);
  gxPromptCatalog.vFuns(PF_TIMERSTART);
  gxPromptCatalog.vFuns(PF_TRACKMEMORY);
  gxPromptCatalog.vFuns(PF_UPTIME);
  gxPromptCatalog.vFuns(PF_VERSION);
  gxPromptCatalog.vFuns(PF_WAITING);

  vTrace("\nsuccess: %lld rows affected\n"_Stz, gxPromptCatalog.m_nFunctions);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
