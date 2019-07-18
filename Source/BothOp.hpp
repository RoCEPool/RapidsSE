/*============================================================================*/
/* BothOp.hpp -- Both                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class BothOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    BothOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~BothOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  BothOp* _pBothOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
