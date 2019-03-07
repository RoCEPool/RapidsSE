/*============================================================================*/
/* Prompt_jobClass.cpp -- print job classes                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobClass (
  PF_JOBCLASS,           // prompt function
  "Prompt_jobClass"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "jobClass"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_jobClass,      // prompt function
  true,                  // enabled

  true,                  // visible
  "jobClass"_Stz,        // help
  ""_Stz                 // description
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
           aCN[pJobClass->m_eClass],
           aJK[pJobClass->m_eKind]
           );
    
    i_JobClass++;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
