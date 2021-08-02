/*============================================================================*/
/* ReqTuple.hpp -- Request Tuple                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ReqTuple
  {
  public:
    i8   m_i_Origin;   // originated on which node
    i8   m_i_Session;  // which session in session array
    i8   m_iCmdNo_tuple;     // the sequence no of the request from that session

  public:
    void* operator new(size_t);
  
    ReqTuple(void);
    ReqTuple(ReqTuple* aReqTuple);
    ReqTuple(Stz* szService, i8 iSessionId);
   ~ReqTuple(void) noexcept(false);

    void vContentDump(void);
    void vSet(ReqTuple* aReqTuple);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
