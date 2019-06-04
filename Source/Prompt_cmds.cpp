/*============================================================================*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cmds (
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
  JobClass::vCmds(CN_JOB_CAT);
  JobClass::vCmds(CN_JOB_CF);
  JobClass::vCmds(CN_JOB_DOCOL);
  JobClass::vCmds(CN_JOB_DUMPINDEX);
  JobClass::vCmds(CN_JOB_DUMPTABLE);
  JobClass::vCmds(CN_JOB_FETCHROWS);
  JobClass::vCmds(CN_JOB_GENERATE);
  JobClass::vCmds(CN_JOB_INFLATEROWS);
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
  JobClass::vCmds(CN_JOB_USESCHEMA);
  JobClass::vCmds(CN_JOB_VISUALIZEINDEX);
  JobClass::vCmds(CN_JOB_VISUALIZETABLE);
  
  // dist2pc
  JobClass::vCmds(CN_JOB_ANALYZEINDEX);
  JobClass::vCmds(CN_JOB_ANALYZETABLE);
  JobClass::vCmds(CN_JOB_COMPACTINDEX);
  JobClass::vCmds(CN_JOB_CONNECTCLUSTER);
  JobClass::vCmds(CN_JOB_CREATECATALOG);
  JobClass::vCmds(CN_JOB_CREATEINDEX);
  JobClass::vCmds(CN_JOB_CREATESCHEMA);
  JobClass::vCmds(CN_JOB_CREATETABLE);
  JobClass::vCmds(CN_JOB_CREATETENANT);
  JobClass::vCmds(CN_JOB_DROPCATALOG);
  JobClass::vCmds(CN_JOB_DROPINDEX);
  JobClass::vCmds(CN_JOB_DROPSCHEMA);
  JobClass::vCmds(CN_JOB_DROPTABLE);
  JobClass::vCmds(CN_JOB_DROPTENANT);
  JobClass::vCmds(CN_JOB_EXPORT);
  JobClass::vCmds(CN_JOB_INSERTROW);
  //JobClass::vCmds(CN_JOB_INSERTTABLE);
  JobClass::vCmds(CN_JOB_LOADTABLE);
  JobClass::vCmds(CN_JOB_LISTPARTITIONS);
  JobClass::vCmds(CN_JOB_PUBLISH);
  JobClass::vCmds(CN_JOB_PURGE);
  JobClass::vCmds(CN_JOB_SANITY);
  JobClass::vCmds(CN_JOB_SIZEINDEX);
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
