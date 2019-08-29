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
Request* Sessions::_pCommand(void* aAddr)
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
Request::Request(Session* p_Session, TokenVec* p_TknVec)
: Focal(CN_REQUEST,AM::iNextRequest())
, m_xReqTuple(nullptr)
{
  _precondition_(_MAGIC_,CN_REQUEST,_FOOTER_);
  
  m_p_Session = p_Session;
  m_p_TknVec  = p_TknVec;
  
  m_xReqTuple.m_iReqId = m_iUniqueId;
  
  _checkInitialised_(CN_REQUEST);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  SP_TknVecStr spTknVecStr;
  
  AM::vLogReq(SL_INF,this,MA_REQ_BEGIN,
              "Request begin: %s"_Stz, //!format-LogReq
              spTknVecStr.sz(m_p_TknVec));

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
  
  AM::vLogReq(SL_INF,this,MA_REQ_END,
              "Request end"_Stz); //!format-LogReq
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
