/*============================================================================*/
/* Prompt_st.cpp -- step tracker                                              */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_st (
  PF_ST,                // prompt function
  "Prompt_st"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "stepTiming"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_st,           // prompt function
  true,                 // enabled

  true,                 // visible
  "st"_Stz,             // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_st(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("stepTiming"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // start step timing
  if ( (*pTokenVec)[1]->bSame("start"_Stz) ) {
    if (AM::pStepTiming()) {
      vTrace("Step timing already exists\n"_Stz);
      return true;
    }
    AM::vStepTiming(new StepTiming);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset step timing
  if ( (*pTokenVec)[1]->bSame("reset"_Stz) ) {
    if (! AM::pStepTiming()) {
      vTrace("No step tracker\n"_Stz);
      return true;
    }
    AM::pStepTiming()->vReset();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write step timings to given file
  if ( (*pTokenVec)[1]->bSame("write"_Stz) ) {
    
    s256 sPath = (*pTokenVec)[2]->szTerm();

    if (! AM::pStepTiming()) {
      vTrace("No step tracker\n"_Stz);
      return true;
    }
    AM::pStepTiming()->vWrite(sPath);
    return true;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
