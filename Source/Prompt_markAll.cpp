/*============================================================================*/
/* Prompt_markAll.cpp -- the mark all option                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_markAll (
  "Prompt_markAll"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markAll"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markAll,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markAll(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();

  if (nTokens > 1) {
  
    Stz* aArg = pPrompt->pTknVec()->pGet(1)->aText();

    if (aArg->bIsSomeTrue() ) {
      pPrompt->vMarkAll(true);
      return true;
    }

    if (aArg->bIsSomeFalse() ) {
      pPrompt->vMarkAll(false);
      return true;
    }

  }

  vTrace("MarkAll is currently : %s\n"_Stz,
         pPrompt->bMarkAll() ? "on"_Stz : "off"_Stz);
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
