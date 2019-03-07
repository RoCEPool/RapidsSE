/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_print (
  PF_PRINT,          // prompt function
  "Prompt_print"_Stz,// name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "print"_Stz,       // the prompt command
  nullptr,           // the second word
  bPrompt_print,     // prompt function
  true,              // enabled

  true,              // visible
  "print"_Stz,       // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_print(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("print"_Stz));
  
  if (pTokenVec->nTokens() > 1)
    vInfo("%s\n"_Stz,pTokenVec->pGet(1)->aText());
  else
    vInfo("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
