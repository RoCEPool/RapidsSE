/*============================================================================*/
/* Prompt_st.cpp -- step tracker                                              */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_st (
  PF_ST,                // prompt function
  "Prompt_st"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "st"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_st,           // prompt function
  true,                 // enabled

  true,                 // visible
  "st"_Stz,              // help
                           ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_st(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("st"_Stz));
  
  // start step tracker
  if ( (*pTokenVec)[1]->bSame("start"_Stz) ) {
    if (gpAttachMap->m_pStepTiming) {
      vTrace("Step timing already exists\n"_Stz);
      return true;
    }
    gpAttachMap->m_pStepTiming = new StepTiming;
    return true;
  }
  
  // reset step tracker
  if ( (*pTokenVec)[1]->bSame("reset"_Stz) ) {
    if (! gpAttachMap->m_pStepTiming) {
      vTrace("No step tracker\n"_Stz);
      return true;
    }
    gpAttachMap->m_pStepTiming->vReset();
    return true;
  }
  
  // write step tracker contents to file
  if ( (*pTokenVec)[1]->bSame("write"_Stz) ) {
    if (! gpAttachMap->m_pStepTiming) {
      vTrace("No step tracker\n"_Stz);
      return true;
    }
    gpAttachMap->m_pStepTiming->vWrite();
    return true;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
