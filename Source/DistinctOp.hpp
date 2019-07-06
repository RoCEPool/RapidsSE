/*============================================================================*/
/* DistinctOp.hpp -- Distinct                                                 */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class DistinctOp : public Op
  {
  private:

  public:
    DistinctOp(void);
   ~DistinctOp(void) noexcept(false);
  };

  DistinctOp* _pDistinctOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
