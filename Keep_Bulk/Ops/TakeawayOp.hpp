/*============================================================================*/
/* TakeawayOp.hpp -- Takeaway                                                 */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class TakeawayOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    TakeawayOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~TakeawayOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  TakeawayOp* _pTakeawayOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
