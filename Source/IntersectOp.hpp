/*============================================================================*/
/* IntersectOp.hpp -- Intersect                                               */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class IntersectOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    IntersectOp(Jobs::Job* pJob);
   ~IntersectOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  IntersectOp* _pIntersectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
