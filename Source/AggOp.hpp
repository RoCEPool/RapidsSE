/*============================================================================*/
/* AggOp.hpp -- Aggregator                                                    */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class AggOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    AggOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~AggOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);  
  };

  AggOp* _pAggOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
