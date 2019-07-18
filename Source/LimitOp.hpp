/*============================================================================*/
/* LimitOp.hpp -- Limit                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class LimitOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    LimitOp(Jobs::Job* pJob);
   ~LimitOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  LimitOp* _pLimitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
