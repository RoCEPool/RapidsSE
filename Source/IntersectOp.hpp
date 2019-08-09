/*============================================================================*/
/* IntersectOp.hpp -- Intersect                                               */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class IntersectOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    IntersectOp(Stz* szOpLabel, Digest* pDigest);
   ~IntersectOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  IntersectOp* _pIntersectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
