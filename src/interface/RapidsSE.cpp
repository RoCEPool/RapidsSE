/*============================================================================*/
/* RapidsSE.cpp -- RapidsSE External Api functions                            */
/*============================================================================*/

#include "Interface.h"

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* RapidsSE::pAllocSession(eSK eSession)
{
  _static_
    
  Session* pSession = Api::pAllocSession(eSession);
    
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vFreeSession(Session* pSession)
{
  _static_
  
  Api::vFreeSession(pSession);
}

//
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
//

/*----------------------------------------------------------------------------*/
/* delete job                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDeleteJob(Session* pSession)
{
  _static_
  
  Api::vDeleteJob(pSession);
}


/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecuteJob(Session* pSession)
{
  _static_
  
  Api::vExecuteJob(pSession);
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE::ePrepareJob(Session*  pSession,
                          TokenVec* pCmdTknVec,
                          TokenVec* pLitTknVec,
                          TagSet*   pExtraTagSet)
{
  _static_

  eEC eError = Api::ePrepareJob(pSession,pCmdTknVec,pLitTknVec,pExtraTagSet);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* submit job (spawn job but don't wait for completion)                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSubmitJob(Session* pSession)
{
  _static_
  
  Api::vSubmitJob(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitJob(Session* pSession)
{
  Api::vWaitJob(pSession);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
