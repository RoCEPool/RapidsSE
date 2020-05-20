/*============================================================================*/
/* Request.cpp -- Request                                                     */
/*============================================================================*/

#include "Sessions.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Request* Sessions::_pRequest(void* aAddr)
{
  Request* pRequest = reinterpret_cast<Request*>(aAddr);
  
  RASSERT_TYPE(pRequest, CN_REQUEST);
  
  return pRequest;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Request::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_REQUEST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Request::operator delete(void* aAddr)
{
  Request* pRequest = reinterpret_cast<Request*>(aAddr);
  
  HeapMem::vFree(pRequest);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Request::Request(Session* p_Session, Stz* szPurpose, i8 iClassSeq)
: Focal(CN_REQUEST,AM::iNextRequest(),p_Session->m_iFocalId)
, m_xReqTuple(nullptr)
{
  _precondition_(_MAGIC_,CN_REQUEST,_FOOTER_);
  
  m_p_Session = p_Session;
  m_p_TknVec  = nullptr;
  
  m_xReqTuple.m_sPurpose.vSprint("%s-%03lld"_Stz,szPurpose,iClassSeq);
  
  m_xReqTuple.m_i_Origin  = AM::iThisNodeNo();
  m_xReqTuple.m_i_Session = p_Session->m_i_Session;

  m_xReqTuple.m_iReqId = m_iFocalId;
  m_xReqTuple.m_iForId = p_Session->m_iFocalId;

  _checkInitialised_(CN_REQUEST);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Now oNow;
  SP_ReqTuple spReqTuple;
  
  vIncStepNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  _postcondition_(_MAGIC_,CN_REQUEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Request::~Request(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_REQUEST,_FOOTER_);
  
  // do not delete: m_p_Session
  // do not delete: m_p_TknVec
  
  Now oNow;
  SP_ReqTuple spReqTuple;

  vIncStepNo();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get request tuple                                                          */
/*----------------------------------------------------------------------------*/
ReqTuple* Request::aReqTuple(void)
{
  _precondition_(_MAGIC_,CN_REQUEST,_FOOTER_);
  
  return & m_xReqTuple;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Request)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
