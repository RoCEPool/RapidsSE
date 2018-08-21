/*============================================================================*/
/* Prompt_help.cpp -- print prompt help                                       */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_help (
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "help"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt post-process function
  true               // enabled
);

extern JobCatalog    gxJobCatalog;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_help(SePrompt* pPrompt)
{  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++) {
    
    if (! gxJobCatalog.m_xJobClass[i_Job].m_bEnabled)
      continue;
    
    vTrace("%s\n"_Stz,gxJobCatalog.m_xJobClass[i_Job].m_aComment);

  }

  vTrace("\n"_Stz);
  
  for (i8 i_Fun = 0; i_Fun < gxPromptCatalog.m_nFunctions; i_Fun++) {
    
    if (! gxPromptCatalog.m_xPromptFun[i_Fun].m_bEnabled)
      continue;

    vTrace("%s\n"_Stz,gxPromptCatalog.m_xPromptFun[i_Fun].m_aComment);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
