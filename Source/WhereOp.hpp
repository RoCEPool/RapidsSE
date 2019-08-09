/*============================================================================*/
/* WhereOp.hpp -- Where                                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class WhereOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    WhereOp(Stz* szOpLabel, Digest* pDigest);
   ~WhereOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  WhereOp* _pWhereOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
