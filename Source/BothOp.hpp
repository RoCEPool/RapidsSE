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
    BothOp(Stz* szOpLabel, Digest* pDigest, Jobs::Job* pJob);
   ~BothOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  BothOp* _pBothOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
