/*============================================================================*/
/* Prompt_markApi.cpp -- the mark api option                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_markApi (
  "Prompt_markApi"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markApi"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markApi,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markApi(SePrompt* pPrompt)
{  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
