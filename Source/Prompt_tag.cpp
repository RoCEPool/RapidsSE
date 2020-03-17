/*============================================================================*/
/* Prompt_tag.cpp -- session tag managememnt                                  */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_tag (
  PF_TAGS,                // prompt function
  "Prompt_tag"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "tag"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_tags,           // prompt function
  true,                   // enabled

  true,                   // visible
  "tag"_Stz,              // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tag(SePrompt* pPrompt)
{
  Session*  pSession  = pPrompt->pSession();

  Semantic* pSemantic = pSession->m_pSemantic;
  
  vTrace("Session Tags:\n"_Stz);

  EnameSet* pEnameSet = pSemantic->pEnameSet();

  pEnameSet->vPrint();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
