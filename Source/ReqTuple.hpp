/*============================================================================*/
/* ReqTuple.hpp -- Request Tuple                                              */
/*============================================================================*/
#pragma once

namespace Focals
{
  class ReqTuple
  {
  public:
    i8   m_i_State;    // which session state
    i8   m_iReqId;     // the id of the request
    i8   m_iForId;     // id of prompt or api
  
    i8   m_iDetail;    // level of logging  
    s32  m_sPurpose;   // purpose of request

  public:
    void* operator new(size_t);
  
    ReqTuple(ReqTuple* aReqTuple);
    ReqTuple(Stz* szService, bool bFlag);
   ~ReqTuple(void) noexcept(false);

    void vSet(ReqTuple* aReqTuple);
  };
  
  static ReqTuple* C_NO_REQTUPLE = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
