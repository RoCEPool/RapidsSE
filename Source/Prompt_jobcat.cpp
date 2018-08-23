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
  
  // system
  gxJobCatalog.vTraceJobs(CN_JOB_BOOT);
  gxJobCatalog.vTraceJobs(CN_JOB_START);
  gxJobCatalog.vTraceJobs(CN_JOB_PROMPT);
  gxJobCatalog.vTraceJobs(CN_JOB_CONNECTOR);
  gxJobCatalog.vTraceJobs(CN_JOB_ANSWER);
  gxJobCatalog.vTraceJobs(CN_JOB_RECEIVER);

  // local
  gxJobCatalog.vTraceJobs(CN_JOB_DOCOL);
  gxJobCatalog.vTraceJobs(CN_JOB_CF);
  gxJobCatalog.vTraceJobs(CN_JOB_LF);
  gxJobCatalog.vTraceJobs(CN_JOB_PWF);
  
  gxJobCatalog.vTraceJobs(CN_JOB_FETCHROWS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTTENANTS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTCATALOGS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTSCHEMAS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTTABLES);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTCOLUMNS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTIDXCOLUMNS);
  gxJobCatalog.vTraceJobs(CN_JOB_LISTPARTCOLUMNS);
  gxJobCatalog.vTraceJobs(CN_JOB_USESCHEMA);
  gxJobCatalog.vTraceJobs(CN_JOB_DUMPINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_VISUALIZEINDEX);

  // generic
  gxJobCatalog.vTraceJobs(CN_JOB__BEGUN);
  gxJobCatalog.vTraceJobs(CN_JOB__EXECUTE);
  gxJobCatalog.vTraceJobs(CN_JOB__EXECUTED);
  gxJobCatalog.vTraceJobs(CN_JOB__END);
  gxJobCatalog.vTraceJobs(CN_JOB__ENDED);
  gxJobCatalog.vTraceJobs(CN_JOB__CANCEL);
  gxJobCatalog.vTraceJobs(CN_JOB__CANCELLED);

  // dist2pc
  gxJobCatalog.vTraceJobs(CN_JOB_ANALYZEINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_ANALYZETABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_COMPACTINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_CREATECATALOG);
  gxJobCatalog.vTraceJobs(CN_JOB_CREATEINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_CREATESCHEMA);
  gxJobCatalog.vTraceJobs(CN_JOB_CREATETABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_CREATETENANT);
  gxJobCatalog.vTraceJobs(CN_JOB_DROPCATALOG);
  gxJobCatalog.vTraceJobs(CN_JOB_DROPINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_DROPSCHEMA);
  gxJobCatalog.vTraceJobs(CN_JOB_DROPTABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_DROPTENANT);
  gxJobCatalog.vTraceJobs(CN_JOB_LOADTABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_SIZEINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_SIZETABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_TABLESIZES);
  gxJobCatalog.vTraceJobs(CN_JOB_TRUNCATETABLE);
  gxJobCatalog.vTraceJobs(CN_JOB_VERIFYINDEX);
  gxJobCatalog.vTraceJobs(CN_JOB_SANITY);
  gxJobCatalog.vTraceJobs(CN_JOB_SPACE);
  gxJobCatalog.vTraceJobs(CN_JOB_STATUS);
  
  // child
  gxJobCatalog.vTraceJobs(CN_JOB_SUBFETCH);
  gxJobCatalog.vTraceJobs(CN_JOB_SUBLOADER);
  gxJobCatalog.vTraceJobs(CN_JOB_SUBTRUNCATE);
  
  // grankid
  gxJobCatalog.vTraceJobs(CN_JOB_LOADBUFFER);

  vTrace("\n"_Stz);
  
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
