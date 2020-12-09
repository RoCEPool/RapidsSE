/*============================================================================*/
/* RapidsSE.cpp -- RapidsSE External Api functions                            */
/*============================================================================*/

#include "Interface.h"

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
void* RapidsSE::aAllocSession(void)
{
  _static_
    
  Session* pSession = Api::pAllocSession(SK_EXTERNAL);
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vFreeSession(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vFreeSession(pSession);
}

//
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
//

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDeleteJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vDeleteJob(pSession);
}


/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecuteJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vExecuteJob(pSession);
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::ePrepareJob(void* aSession, char* azCommand)
{
  _static_

  TokenVec* pCmdTknVec = new TokenVec;;
  TokenVec* pLitTknVec = new TokenVec;;
  TagSet    oExtTagSet;

  Session* pSession = _pSession(aSession);
  
  pCmdTknVec->vTokeniseC((Stz*)azCommand);
  pCmdTknVec->vDumpVec(0);

  oExtTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  oExtTagSet.vReplace(C_TAG_TRACEOP,1);
  
  eEC eError = Api::ePrepareJob(pSession,pCmdTknVec,pLitTknVec,&oExtTagSet);
  
  delete pCmdTknVec;
  delete pLitTknVec;

  return eError;
}

/*----------------------------------------------------------------------------*/
/* submit job (spawn job but don't wait for completion)                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSubmitJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vSubmitJob(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitJob(void* aSession)
{
  Session* pSession = _pSession(aSession);
  Api::vWaitJob(pSession);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
