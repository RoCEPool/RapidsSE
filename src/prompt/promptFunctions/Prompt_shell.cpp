/*============================================================================*/
/* Prompt_shell.cpp -- shell command                                          */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_shell (
  PF_SHELL,           // prompt function
  "Prompt_shell"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "shell"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_shell,      // prompt function
  true,               // enabled

  true,               // visible
  "shell"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_shell(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  s4K sCommand;
  
  i8 nTokens = pTknVec->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 1; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
      
      case TN_INTEGER:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_NAME:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_OTHER:
        sCommand.vConcat(pToken->szChars());
        break;

      case TN_STRING:
        //sCommand.vConcat("\""_Stz);
        sCommand.vConcat(pToken->szChars());
        //sCommand.vConcat("\""_Stz);
        break;
        
      default:
        NEVER_REACH;
    }
    
    sCommand.vConcat(" "_Stz);
  }
  
  // concat a terminating semicolon (needed??)
  sCommand.vConcat(";"_Stz);

  vTrace("Shell:: %s\n"_Stz,sCommand.szStz());
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(sCommand.szStz(),oSysCall);
  
  return iExitStatus ? true : false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
