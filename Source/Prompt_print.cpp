/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_print (
  "Prompt_print"_Stz,// name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "print"_Stz,       // the prompt command
  nullptr,           // the second word
  bPrompt_print,        // prompt post-process function
  true               // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_print(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("print"_Stz));
  
  vInfo("%s\n"_Stz, pTokenVec->pGet(1)->aText());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
