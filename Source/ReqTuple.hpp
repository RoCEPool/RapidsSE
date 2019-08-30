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
    i8   m_iReqId;     // the command which trigger the job
    bool m_bMarkTrace; // trace (mark version)
    bool m_bLogJob;    // mark activity
  
    s32  m_sPurpose;   // purpose of request

  public:
    void* operator new(size_t);
  
    ReqTuple(ReqTuple* aReqTuple);
   ~ReqTuple(void) noexcept(false);

    void vSet(ReqTuple* aReqTuple);
  };
  
  static ReqTuple* C_NO_REQTUPLE = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
