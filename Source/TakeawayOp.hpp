/*============================================================================*/
/* TakeawayOp.hpp -- Takeaway                                                 */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class TakeawayOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    TakeawayOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~TakeawayOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  TakeawayOp* _pTakeawayOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
