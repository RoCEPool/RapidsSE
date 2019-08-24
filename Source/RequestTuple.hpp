/*============================================================================*/
/* RequestTuple.hpp -- Request Tuple                                          */
/*============================================================================*/
#pragma once

namespace Sessions
{
  class RequestTuple
  {
  public:
  
  public:
    void* operator new(size_t);
  
    RequestTuple(RequestTuple* aRequestTuple);
   ~RequestTuple(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
