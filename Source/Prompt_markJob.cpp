/*============================================================================*/
/* Prompt_markJob.cpp -- the mark job option                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_markJob (
  "Prompt_markJob"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markJob"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markJob,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markJob(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();

  if (nTokens > 1) {
  
    Stz* aArg = pPrompt->pTknVec()->pGet(1)->aText();

    if (aArg->bSame("on"_Stz)   ||
        aArg->bSame("1"_Stz)    ||
        aArg->bSame("true"_Stz) ||
        aArg->bSame("t"_Stz) ) {
      pPrompt->vMarkAll(true);
      return true;
    }

    if (aArg->bSame("off"_Stz)   ||
        aArg->bSame("0"_Stz)    ||
        aArg->bSame("false"_Stz) ||
        aArg->bSame("f"_Stz) ) {
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
