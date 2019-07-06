/*============================================================================*/
/* SortOp.hpp -- Sort                                                        */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class SortOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    SortOp(void);
   ~SortOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  SortOp* _pSortOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
