/*============================================================================*/
/* Prompt_jobClass.cpp -- print job classes                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobClass (
  "Prompt_jobClass"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "jobClass"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_jobClass,      // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobClass(SePrompt* pPrompt)
{
  
  JobClass* pJobClass = nullptr;
  
  i8 i_JobClass = 0;
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    vTrace("  %02lld %-20s %s\n"_Stz,
           i_JobClass,
           pJobClass->m_aName,
           aJK[pJobClass->m_eKind]
           );
    
    i_JobClass++;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
