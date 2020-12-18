/*============================================================================*/
/* Prompt_procedure.cpp -- define a stored procedure                          */
/*============================================================================*/

#include "StorageEngine.h"

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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  //pInpTknVec->vListTokens();
  
  pInpTknVec->vIntegrityCheck();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("procedure"_Stz));
  
  Stz* ssProcNm = pInpTknVec->pGet(1)->szChars();
  
  Procedure::vAddProcedure(pInpTknVec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
