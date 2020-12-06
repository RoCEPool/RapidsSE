/*============================================================================*/
/* Prompt_apiDemo.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_apiDemo (
  PF_APIDEMO,           // prompt function
  "Prompt_apiDemo"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "apiDemo"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_apiDemo,      // prompt function
  true,                 // enabled

  true,                 // visible
  "apiDemo"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_apiDemo(SePrompt* pPrompt)
{
  vTrace("Api Demo\n"_Stz);
  
  eEC eError;
  
  TokenVec* pInpTknVec = new TokenVec;
  TokenVec* pLitTknVec = new TokenVec;
  TagSet    oExtTagSet;

  pInpTknVec->vTokeniseC("tableScan bolTbl"_Stz);
  pInpTknVec->vDumpVec(0);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::pAllocSession\n"_Stz);
  Session* pSession = Api::pAllocSession(SK_EXTERNAL);
  if (! pSession) {
    vTrace("  Api::pAllocSession error: unable to allocate session\n"_Stz);
    goto L_Cleanup;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::ePrepareJob\n"_Stz);
  eError = Api::ePrepareJob(pSession,
                            pInpTknVec,
                            pLitTknVec,
                            &oExtTagSet);
  if (eError) {
    vTrace("  Api::ePrepareJob error: %s\n"_Stz,aEC[eError]);
    goto L_End;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_End:
  vTrace("  Api::vFreeSession\n"_Stz);
  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_Cleanup:
  delete pInpTknVec;
  delete pLitTknVec;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
