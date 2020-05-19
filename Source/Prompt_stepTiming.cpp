/*============================================================================*/
/* Prompt_stepTiming.cpp -- step timing                                       */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_st (
  PF_ST,                // prompt function
  "Prompt_st"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "stepTiming"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_stepTiming,   // prompt function
  true,                 // enabled

  true,                 // visible
  "stepTiming"_Stz,     // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_stepTiming(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 nTokens = pInpTknVec->nTokens();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("stepTiming"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (1 == nTokens) {
    if (AM::pStepTiming())
      vTrace("Step timing started\n"_Stz);
    else
      vTrace("Step timing not started\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // start step timing
  if ( pInpTknVec->pGet(1)->bSame("start"_Stz) ) {
    if (AM::pStepTiming()) {
      vTrace("Step timing already started\n"_Stz);
      return true;
    }
    AM::vStepTiming(new StepTiming);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset step timing
  if ( pInpTknVec->pGet(1)->bSame("reset"_Stz) ) {
    if (! AM::pStepTiming()) {
      vTrace("Step timing not started\n"_Stz);
      return true;
    }
    AM::pStepTiming()->vReset();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // show a step timing
  if ( pInpTknVec->pGet(1)->bSame("show"_Stz) && nTokens == 3) {
    
    s32 sStepNm = pInpTknVec->pGet(2)->szChars();

    if (! AM::pStepTiming()) {
      vTrace("Step timing not started\n"_Stz);
      return true;
    }
    AM::pStepTiming()->vShow(sStepNm);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write step timings to given file
  if ( pInpTknVec->pGet(1)->bSame("write"_Stz) && nTokens == 3) {
    
    s256 sPath = pInpTknVec->pGet(2)->szChars();

    if (! AM::pStepTiming()) {
      vTrace("Step timing not started\n"_Stz);
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
