/*============================================================================*/
/* Prompt_opCat.cpp -- print op catalog                                       */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_opCat (
  PF_OPCAT,            // prompt function
  "Prompt_opCat"_Stz,  // name of the prompt function
  "comment"_Stz,       // comment about the function
  "opcat"_Stz,         // the prompt command
  nullptr,             // the second word
  bPrompt_opCat,       // prompt function
  true,                // enabled

  true,                // visible
  "opcat"_Stz,         // help
  ""_Stz               // description
);

//extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_opCat(SePrompt* pPrompt)
{
  //gxOpCatalog.vPrintCatalog();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
