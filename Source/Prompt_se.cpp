/*============================================================================*/
/* Prompt_se.cpp -- se interface test                                         */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_se (
  PF_NONE,                     // prompt function
  "Prompt_se"_Stz, // name of the prompt function
  "comment"_Stz,   // comment about the functopm
  "se"_Stz,        // the prompt command
  nullptr,         // the second word
  bPrompt_se,      // prompt function
  true,            // enabled

  true,            // visible
  "fun help"_Stz   // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_se(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pTknVec();
  
  s4K sCommand;
  
  i8 nTokens = pTknVec->nTokens();
  
  // silently ignore: se ;
  if (1 == nTokens)
    return true;
  
  // concat all the tokens together
  for (i8 i_Token = 1; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
      
      case TN_INTEGER:
        sCommand.vConcat(pToken->aText());
        break;
        
      case TN_NAME:
        sCommand.vConcat(pToken->aText());
        break;
        
      case TN_OTHER:
        sCommand.vConcat(pToken->aText());
        break;

      case TN_STRING:
        sCommand.vConcat("\""_Stz);
        sCommand.vConcat(pToken->aText());
        sCommand.vConcat("\""_Stz);
        break;
        
      default:
        NEVER_REACH;
    }
    
    sCommand.vConcat(" "_Stz);
  }
  
  // concat a terminating semicolon (needed??)
  sCommand.vConcat(";"_Stz);

  vTrace("SE:: %s\n"_Stz,sCommand.aStz());
  
  // call se to execute command
  Job* pJob = Api::pExecuteCommand(pPrompt->pSession(),sCommand,true,true);
  
  if (pJob) {
    delete pJob;
    return true;
  }
  else {
    return false;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
