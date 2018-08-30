/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobcat (
  "Prompt_jobcat"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "jobcat"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_jobcat,      // prompt post-process function
  true                 // enabled
);

extern JobCatalog    gxJobCatalog;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
{
  JobClass::vJobCatHeader();
  
  i8 nJobs = 0;
  
  // thread
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_BOOT);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_PROMPT);

  // system
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_START);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CONNECTOR);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_ANSWER);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_RECEIVER);

  // local
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DOCOL);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CF);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LF);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_PWF);
  
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTTENANTS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTCATALOGS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTSCHEMAS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTTABLES);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTCOLUMNS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTINDEXES);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LISTPARTITIONS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_USESCHEMA);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SHOWCLUSTER);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DUMPINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_VISUALIZEINDEX);

  // generic
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__BEGUN);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__EXECUTE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__EXECUTED);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__END);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__ENDED);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__CANCEL);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB__CANCELLED);

  // dist2pc
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_ANALYZEINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_ANALYZETABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_COMPACTINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CREATECATALOG);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CREATEINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CREATESCHEMA);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CREATETABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_CREATETENANT);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DROPCATALOG);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DROPINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DROPSCHEMA);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DROPTABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_DROPTENANT);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LOADTABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_FETCHROWS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SIZEINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SIZETABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_TABLESIZES);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_TRUNCATETABLE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_VERIFYINDEX);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SANITY);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SPACE);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_STATUS);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_STARTCLUSTER);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_WAITCLUSTER);

  // child
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SUBLOADER);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SUBFETCH);
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; gxJobCatalog.vTraceJobs(CN_JOB_LOADBUFFER);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WAITCLUSTER - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs\n\n"_Stz);

  return true;
}
/*----------------------------------------------------------------------------*/
//bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
//{
//  JobClass::vJobCatHeader();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_SYSTEM == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_LOCAL == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_GENERIC == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_DIST2PC == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_CHILD == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
//    if (JK_GRANDKID == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
//      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
//
//  vTrace("\n"_Stz);
//
//  return true;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
