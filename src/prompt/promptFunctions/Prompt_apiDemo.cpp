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

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  
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
  vTrace("  Api::vExecuteJob\n"_Stz);
  Api::vExecuteJob(pSession->m_pSsnJob);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Process Results\n"_Stz);
  if (pSession->m_pSsnJob->bIsPipeline())
    pSession->m_pSsnJob->m_xResult.vProcessBufferQueue();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("  Api::vDeleteJob\n"_Stz);
  Api::vDeleteJob(&pSession->m_pSsnJob);

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
