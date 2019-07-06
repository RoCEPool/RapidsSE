/*============================================================================*/
/* IntersectOp.hpp -- Intersect                                               */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class IntersectOp : public Op
  {
  private:

  public:
    IntersectOp(void);
   ~IntersectOp(void) noexcept(false);
  };

  IntersectOp* _pIntersectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
