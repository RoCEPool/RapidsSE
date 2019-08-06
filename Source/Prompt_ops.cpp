/*============================================================================*/
/* Prompt_ops.cpp -- prompt op list function                                  */
/*============================================================================*/

#include "Prompt.h"

extern OpCatalog gxOpCatalog;

PromptDesc __Prompt_ops (
  PF_FUNS,               // prompt function
  "Prompt_ops"_Stz,      // name of the prompt function
  "comment"_Stz,         // comment about the function
  "ops"_Stz,             // the prompt command
  nullptr,               // the second word
  bPrompt_ops,           // prompt function
  true,                  // enabled

  true,                  // visible
  "ops"_Stz,             // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ops(SePrompt* pPrompt)
{
  gxOpCatalog.vHelp(CN_FETCHOP);
  gxOpCatalog.vHelp(CN_OUTPUTOP);
  gxOpCatalog.vHelp(CN_SCANOP);
  gxOpCatalog.vHelp(CN_ZEDOP);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
