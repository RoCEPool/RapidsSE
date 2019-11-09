/*============================================================================*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_cmds (
  PF_CMDS,               // prompt function
  "Prompt_cmds"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "cmds"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_cmds,          // prompt function
  true,                  // enabled

  true,                  // visible
  "cmds"_Stz,            // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cmds(SePrompt* pPrompt)
{
  
  // local
  JobClass::vCmds(CN_JOB_PIPELINE);
  JobClass::vCmds(CN_JOB_FETCHROWS);
  
  // dist2pc
  JobClass::vCmds(CN_JOB_ANALYZEINDEX);
  JobClass::vCmds(CN_JOB_ANALYZETABLE);
  JobClass::vCmds(CN_JOB_COMPACTINDEX);
  JobClass::vCmds(CN_JOB_CONNECTCLUSTER);
  JobClass::vCmds(CN_JOB_CREATEINDEX);
  JobClass::vCmds(CN_JOB_CREATETABLE);
  JobClass::vCmds(CN_JOB_EXPORT);
  JobClass::vCmds(CN_JOB_INSERTROW);
  //JobClass::vCmds(CN_JOB_INSERTTABLE);
  JobClass::vCmds(CN_JOB_LOADTABLE);
  JobClass::vCmds(CN_JOB_PUBLISH);
  JobClass::vCmds(CN_JOB_PURGE);
  JobClass::vCmds(CN_JOB_SANITY);
  JobClass::vCmds(CN_JOB_SIZETABLE);
  JobClass::vCmds(CN_JOB_SPACE);
  JobClass::vCmds(CN_JOB_STARTCLUSTER);
  JobClass::vCmds(CN_JOB_STATUS);
  JobClass::vCmds(CN_JOB_TABLESIZES);
  JobClass::vCmds(CN_JOB_TRUNCATETABLE);
  JobClass::vCmds(CN_JOB_VERIFYINDEX);
  JobClass::vCmds(CN_JOB_VERIFYTABLE);
  JobClass::vCmds(CN_JOB_WAITCLUSTER);
  JobClass::vCmds(CN_JOB_WALK);
  

  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
