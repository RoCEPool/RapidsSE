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
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_help(SePrompt* pPrompt)
{
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = JobClass::pFirstFun(); pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass->m_bEnabled )
      continue;

    vTrace("%s\n"_Stz,pJobClass->m_aComment);
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
