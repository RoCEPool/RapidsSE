/*============================================================================*/
/* Prompt_memTest.cpp -- memory pattern test                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_memTest (
  PF_MEMTEST,           // prompt function
  "Prompt_memTest"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "memTest"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_memTest,      // prompt function
  true,                 // enabled

  true,                 // visible
  "memTest"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memTest(SePrompt* pPrompt)
{
  MemTest* pMemTest = new MemTest;
  
  MemLib::vDumpWords(pMemTest,sizeof(MemTest));
  
  i8 iCmp = MemLib::iCompareWords(pMemTest->m_caCharLowerFlipped,
                                  pMemTest->m_caCharUpperFlipped,16);
  
  vTrace("%lld\n"_Stz,iCmp);
  
  delete pMemTest;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
