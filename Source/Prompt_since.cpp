/*============================================================================*/
/* Prompt_since.cpp -- show since                                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_since (
  PF_SINCE,           // prompt function
  "Prompt_since"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "since"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_since,      // prompt function
  true,               // enabled

  true,               // visible
  "since"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_since(SePrompt* pPrompt)
{
  Since oSince;
  oSince.vNow();

  vTrace("%lld.%lld\n"_Stz,oSince.iSeconds(),oSince.iMicroseconds());
  vTrace("%x.%x\n"_Stz,oSince.iSeconds(),oSince.iMicroseconds());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
