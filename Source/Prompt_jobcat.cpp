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
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_SYSTEM == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_LOCAL == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_GENERIC == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_DIST2PC == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_CHILD == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++)
    if (JK_GRANDKID == gxJobCatalog.m_xJobClass[i_Job].m_eKind)
      gxJobCatalog.m_xJobClass[i_Job].vJobCat();
  
  vTrace("\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
