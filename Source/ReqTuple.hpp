/*============================================================================*/
/* ReqTuple.hpp -- Request Tuple                                              */
/*============================================================================*/
#pragma once

namespace Sessions
{
  class ReqTuple
  {
  public:
    i8 m__iDummy;
  
  public:
    void* operator new(size_t);
  
    ReqTuple(ReqTuple* aReqTuple);
   ~ReqTuple(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
