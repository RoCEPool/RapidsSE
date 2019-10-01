/*============================================================================*/
/* WhereOp.hpp -- Where                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class WhereOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    WhereOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~WhereOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  WhereOp* _pWhereOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
