/*============================================================================*/
/* Prompt_displayOff.cpp -- turn display off                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_displayOff (
  "Prompt_displayOff"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the functopm
  "displayOff"_Stz,                // the prompt command
  nullptr,                 // the second word
  bPrompt_displayOff,      // prompt post-process function
  true                     // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_displayOff(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->m_pCommandProcessor->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("displayOff"_Stz));
  
  SEitf* pSEitf = pPrompt->pSEitf();
  pSEitf->m_bDisplay = false;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
