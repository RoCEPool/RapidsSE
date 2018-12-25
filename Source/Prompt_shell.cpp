/*============================================================================*/
/* Prompt_shell.cpp -- shell command                                          */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_shell (
  "Prompt_shell"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "$"_Stz,            // the prompt command
  nullptr,            // the second word
  bPrompt_shell,      // prompt function
  true,               // enabled

  true,               // visible
  "fun help"_Stz      // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_shell(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pTknVec();
  
  s4K sCommand;
  
  i8 nTokens = pTknVec->nTokens();
  
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
        //sCommand.vConcat("\""_Stz);
        sCommand.vConcat(pToken->aText());
        //sCommand.vConcat("\""_Stz);
        break;
        
      default:
        NEVER_REACH;
    }
    
    sCommand.vConcat(" "_Stz);
  }
  
  // concat a terminating semicolon (needed??)
  sCommand.vConcat(";"_Stz);

  vTrace("Shell:: %s\n"_Stz,sCommand.aStr());
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(sCommand.aStr(),oSysCall);
  
  return iExitStatus ? true : false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
