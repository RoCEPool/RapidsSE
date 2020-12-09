/*============================================================================*/
/* RapidsSE.cpp -- RapidsSE External Api functions                            */
/*============================================================================*/

#include "Interface.h"

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eAllocSession(void** aaSession)
{
  _static_
    
  * aaSession = Api::pAllocSession(SK_EXTERNAL);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eFreeSession(void** aaSession)
{
  _static_
  
  Session* pSession = _pSession(*aaSession);
  * aaSession = nullptr;
  Api::vFreeSession(pSession);
  return EC_NONE;
}

//
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § job
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § job
//

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eDeleteJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vDeleteJob(pSession);
  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eExecuteJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vExecuteJob(pSession);
  return EC_NONE;
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
eEC RapidsSE::eSubmitJob(void* aSession)
{
  _static_
  
  Session* pSession = _pSession(aSession);
  Api::vSubmitJob(pSession);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eWaitJob(void* aSession)
{
  Session* pSession = _pSession(aSession);
  Api::vWaitJob(pSession);
  return EC_NONE;
}

//
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § operand
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § operand
//

/*----------------------------------------------------------------------------*/
/* push operand to input op                                                   */
/*----------------------------------------------------------------------------*/
Operand* RapidsSE::pPopOperand(void* aSession,
                               eCN* aeClassNo,
                               eOM* aeOpMarker)
{
  Session* pSession = _pSession(aSession);
  
  Job* pJob = pSession->pJob();

  Operand* pOperand = pJob->m_xResult.pPopBufferQueue_blocking();

  eCN eClassNo;
  eOM eOpMarker;
  
  Result::vDecode(pOperand,eClassNo,eOpMarker);
  
  * aeClassNo  = eClassNo;
  * aeOpMarker = eOpMarker;
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* process operand                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eProcessOperand(void* aSession, void** aaOperand)
{
  Session* pSession = _pSession(aSession);
  Operand* pOperand = _pOperand(*aaOperand);
  *aaOperand = nullptr;

  Job* pJob = pSession->pJob();

  bool b = pJob->m_xResult.bProcessOperand(pOperand);
  
  if (b)
    return EC_NONE;
  else
    return EC_DONE;
}

/*----------------------------------------------------------------------------*/
/* push operand to input op                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::ePushOperand(void* aSession, void** aaOperand)
{
  Session* pSession = _pSession(aSession);
  Operand* pOperand = _pOperand(*aaOperand);
  *aaOperand = nullptr;

  Api::vPushOperand(pSession,pOperand);
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
