/*============================================================================*/
/* Prompt_select.cpp -- select prompt function                                */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_select (
  PF_SHELL,            // prompt function
  "Prompt_select"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "select"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_select,      // prompt function
  true,                // enabled

  true,                // visible
  "select"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_select(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  s48K sSelect;
  
//  i8 nTokens = pTknVec->nTokens();
//
//  // concat all the tokens together
//  for (i8 i_Token = 1; i_Token < nTokens; i_Token++) {
//
//    Token* pToken = pTknVec->pGet(i_Token);
//
//    eTN eToken = pToken->eToken();
//
//    switch (eToken) {
//
//      case TN_INTEGER:
//        sCommand.vConcat(pToken->szChars());
//        break;
//
//      case TN_NAME:
//        sCommand.vConcat(pToken->szChars());
//        break;
//
//      case TN_OTHER:
//        sCommand.vConcat(pToken->szChars());
//        break;
//
//      case TN_STRING:
//        //sCommand.vConcat("\""_Stz);
//        sCommand.vConcat(pToken->szChars());
//        //sCommand.vConcat("\""_Stz);
//        break;
//
//      default:
//        NEVER_REACH;
//    }
//
//    sCommand.vConcat(" "_Stz);
//  }
//
//  // concat a terminating semicolon (needed??)
//  sCommand.vConcat(";"_Stz);

  pTknVec->vToTknString(sSelect);
  vTrace("Select:: %s\n"_Stz,sSelect.szStz());
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
