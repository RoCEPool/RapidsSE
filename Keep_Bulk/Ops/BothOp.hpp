/*============================================================================*/
/* BothOp.hpp -- Both                                                         */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class BothOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    BothOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~BothOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  BothOp* _pBothOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
