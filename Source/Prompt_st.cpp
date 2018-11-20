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
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("st"_Stz));
  
  // start step tracker
  if ( (*pTokenVec)[1]->bSame("start"_Stz) ) {
    gpAttachMap->m_pStepTracker = new StepTracker;
    return true;
  }
  
  // list step tracker contents to file
  if ( (*pTokenVec)[1]->bSame("list"_Stz) ) {
    File xFile("/tmp/StepTracker.csv"_Stz);
    gpAttachMap->m_pStepTracker->vList(xFile);
    return true;
  }
  
  // reset step tracker
  if ( (*pTokenVec)[1]->bSame("reset"_Stz) ) {
    gpAttachMap->m_pStepTracker->vReset();
    return true;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
