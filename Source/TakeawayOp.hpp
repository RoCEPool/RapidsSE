/*============================================================================*/
/* TakeawayOp.hpp -- Takeaway                                                 */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class TakeawayOp : public Op
  {
  private:
  
  public:
    TakeawayOp(void);
   ~TakeawayOp(void) noexcept(false);
  };

  TakeawayOp* _pTakeawayOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
