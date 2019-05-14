/*============================================================================*/
/* Prompt_setDbg.cpp -- set debug                                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_setDbg (
  PF_SETDBG,           // prompt function
  "Prompt_setDbg"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "setDbg"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_setDbg,      // prompt function
  true,                // enabled

  true,               // visible
  "setDbg"_Stz,       // help
  ""_Stz              // description
);

//union {
//
//  struct {
//    i8 m_i1;
//    i8 m_i2;
//  };
//
//  i8 m_iaArray[2];
//
//} u;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_setDbg(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("setDbg"_Stz));
  
  //vTrace(" sizof(u) %lld\n"_Stz,sizeof(u));
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
