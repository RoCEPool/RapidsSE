/*============================================================================*/
/* Prompt_st.cpp -- step tracker                                              */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_st (
  "Prompt_st"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "st"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_st,           // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_st(SePrompt* pPrompt)
{
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
