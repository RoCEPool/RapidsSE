/*============================================================================*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cmds (
  "Prompt_cmds"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "cmds"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_cmds,          // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cmds(SePrompt* pPrompt)
{
  
  // local
  JobClass::vCmds(CN_JOB_CAT);
  JobClass::vCmds(CN_JOB_CF);
  JobClass::vCmds(CN_JOB_COPYBUFFER);
  JobClass::vCmds(CN_JOB_DOCOL);
  JobClass::vCmds(CN_JOB_DUMPINDEX);
  JobClass::vCmds(CN_JOB_FETCHROWS);
  JobClass::vCmds(CN_JOB_GENERATE);
  JobClass::vCmds(CN_JOB_INGEST);
  JobClass::vCmds(CN_JOB_LF);
  JobClass::vCmds(CN_JOB_LISTCATALOGS);
  JobClass::vCmds(CN_JOB_LISTCOLUMNS);
  JobClass::vCmds(CN_JOB_LISTIDXCOLUMNS);
  JobClass::vCmds(CN_JOB_LISTINDEXES);
  JobClass::vCmds(CN_JOB_LISTPARTCOLUMNS);
  JobClass::vCmds(CN_JOB_LISTSCHEMAS);
  JobClass::vCmds(CN_JOB_LISTTABLES);
  JobClass::vCmds(CN_JOB_LISTTENANTS);
  JobClass::vCmds(CN_JOB_MONITOR);
  JobClass::vCmds(CN_JOB_PING);
  JobClass::vCmds(CN_JOB_PWF);
  JobClass::vCmds(CN_JOB_REPEATER);
  JobClass::vCmds(CN_JOB_REPORT);
  JobClass::vCmds(CN_JOB_SELECTTABLE);
  JobClass::vCmds(CN_JOB_SHOWCLUSTER);
  JobClass::vCmds(CN_JOB_SINK);
  JobClass::vCmds(CN_JOB_SUBINSERTER);
  JobClass::vCmds(CN_JOB_USESCHEMA);
  JobClass::vCmds(CN_JOB_VISUALIZEINDEX);
  

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
