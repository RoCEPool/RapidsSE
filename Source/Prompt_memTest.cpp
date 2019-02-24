/*============================================================================*/
/* Prompt_memTest.cpp -- memory pattern test                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_memTest (
  "Prompt_memTest"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "memTest"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_memTest,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memTest(SePrompt* pPrompt)
{
  MemTest* pMemTest = new MemTest;
  
  MemLib::vDumpWords(pMemTest,sizeof(MemTest));
  
  delete pMemTest;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
