/*============================================================================*/
/* RapidsSE.cpp -- RapidsSE External Api functions                            */
/*============================================================================*/

#include "BASE.h"

#include "Output.hpp"
#include "Session.hpp"
#include "Job.hpp"
#include "RapidsSE.hpp"
#include "Api.hpp"
#include "Marker.hpp"

#include "ePP.h"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
char* RapidsSE::aErrorText(eEC eError)
{
  _static_
    
  return (char*)aEC[eError];
}

//
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § session
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § session
//

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

  TokenVec* pLitTknVec = new TokenVec;;

  Session* pSession = _pSession(aSession);
  
  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();

  pCmdTknVec->vTokeniseC((Stz*)azCommand);

  TagSet* pTagSet =  & pSession->m_oJobTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);
  eEC eError = Api::ePrepareJob(pSession,pLitTknVec);

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
/* delete operand                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eDeleteOperand(void* aSession, void** aaOperand)
{
  UNUSED(aSession);
  
  Operand* pOperand = _pOperand(*aaOperand);
  *aaOperand = nullptr;

  AM::vDeleteOperand(pOperand);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* push operand to input op                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::ePopOperand(void* aSession, void** aaOperand,
                          eCN* aeClassNo, eOM* aeOpMarker)
{
  Session* pSession = _pSession(aSession);
  
 
  Operand* pOperand = pSession->m_pOutput->pPopBufferQueue_blocking();

  eCN eClassNo;
  eOM eOpMarker;
  
  vDecode(pOperand,eClassNo,eOpMarker);
  
  * aaOperand  = pOperand;
  * aeClassNo  = eClassNo;
  * aeOpMarker = eOpMarker;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* process operand                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::eProcessOperand(void* aSession, void** aaOperand)
{
  Session* pSession = _pSession(aSession);
  Operand* pOperand = _pOperand(*aaOperand);
  *aaOperand = nullptr;

  bool b = pSession->m_pOutput->bProcessOperand(pOperand);
  
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

/*----------------------------------------------------------------------------*/
/* -- Private --------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode operand into class and marker                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDecode(Operand* pOperand, eCN& eClassNo, eOM& eOpMarker)
{
  _static_

  eClassNo = pOperand->eGetClassNo();

  if (CN_MARKER == eClassNo) {
    Marker* pMarker = _pMarker(pOperand);

    eOpMarker = pMarker->eOpMarker();
  }
  else {
    eOpMarker = OM_NONE;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
