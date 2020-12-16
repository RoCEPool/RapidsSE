/*============================================================================*/
/* ReqTuple.hpp -- Request Tuple                                              */
/*============================================================================*/
#pragma once

namespace Resources
{
  class ReqTuple
  {
  public:
    i8   m_i_Origin;   // originated on which node
    i8   m_i_Session;  // which session in session array
    i8   m_iReqNo;     // the sequence no of the request from that session
    i8   m_iForId;     // id of prompt or api
  
    i8   m_iDetail;    // level of logging  
    s32  m_sPurpose;   // purpose of request

  public:
    void* operator new(size_t);
  
    ReqTuple(void);
    ReqTuple(ReqTuple* aReqTuple);
    ReqTuple(Stz* szService, bool bFlag);
    ReqTuple(Stz* szService, i8 iSessionId, bool bFlag);
   ~ReqTuple(void) noexcept(false);

    void vContentDump(void);
    void vSet(ReqTuple* aReqTuple);
  };
  
  static ReqTuple* C_NO_REQTUPLE = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
