/*============================================================================*/
/* Prompt_displayOn.cpp -- turn display on                                    */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_displayOn (
  "Prompt_displayOn"_Stz,  // name of the prompt function
  "comment"_Stz,           // comment about the functopm
  "displayOn"_Stz,         // the prompt command
  nullptr,                 // the second word
  bPrompt_displayOn,       // prompt function
  true,                    // enabled

  true,                    // visible
  "fun help"_Stz           // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_displayOn(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("displayOn"_Stz));
  
  SEitf* pSEitf = pPrompt->pSEitf();
  pSEitf->m_bDisplay = true;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
