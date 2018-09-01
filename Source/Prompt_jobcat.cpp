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

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
{
  JobClass_o::vJobCatHeader();
  
  i8 nJobs = 0;
  
  // thread
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_BOOT);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_PROMPT);

  // system
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_START);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CONNECTOR);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_ANSWER);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_RECEIVER);

  // local
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DOCOL);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CF);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LF);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_PWF);
  
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTTENANTS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTCATALOGS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTSCHEMAS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTTABLES);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTINDEXES);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LISTPARTITIONS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_USESCHEMA);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SHOWCLUSTER);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DUMPINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_VISUALIZEINDEX);

  // generic
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__BEGUN);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__EXECUTE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__EXECUTED);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__END);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__ENDED);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__CANCEL);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB__CANCELLED);

  // dist2pc
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_ANALYZEINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_ANALYZETABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_COMPACTINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CREATECATALOG);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CREATEINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CREATESCHEMA);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CREATETABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_CREATETENANT);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DROPCATALOG);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DROPINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DROPSCHEMA);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DROPTABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_DROPTENANT);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LOADTABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_FETCHROWS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SIZEINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SIZETABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_TABLESIZES);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_TRUNCATETABLE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_VERIFYINDEX);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SANITY);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SPACE);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_STATUS);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_STARTCLUSTER);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_WAITCLUSTER);

  // child
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SUBLOADER);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SUBFETCH);
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; JobCatalog::vTraceJobs_(CN_JOB_LOADBUFFER);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WAITCLUSTER - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs\n\n"_Stz);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  JobCatalog::vTraceJobs_JC2(CN_JOB__BEGUN);

  return true;
}
/*----------------------------------------------------------------------------*/
//bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
//{
//  JobClass_o::vJobCatHeader();
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
