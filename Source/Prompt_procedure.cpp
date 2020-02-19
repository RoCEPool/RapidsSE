/*============================================================================*/
/* Prompt_procedure.cpp -- define a stored procedure                          */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_procedure (
  PF_PROCEDURE,           // prompt function
  "Prompt_procedure"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "procedure"_Stz,    // the prompt command
  nullptr,            // the second word
  bPrompt_procedure,  // prompt function
  true,               // enabled

  true,               // visible
  "procedure"_Stz,    // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_procedure(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("procedure"_Stz));
  
  s32 sProcNm = (*pTokenVec)[1]->szTerm();
  
  s4K sProcText;
  pTokenVec->vVecToString(sProcText,0,false);
  
  Procedure::vAddProcedure(sProcNm,sProcText);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
