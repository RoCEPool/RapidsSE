/*============================================================================*/
/* AggOp.hpp -- Aggregator                                                    */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class AggOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    AggOp(Stz* szOpLabel, Digest* pDigest);
   ~AggOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);  
  };

  AggOp* _pAggOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
