/*============================================================================*/
/* XF_Dedupe_colNm.hpp -- De-Duplicate Column Name                            */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Dedupe_colNm : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Dedupe_colNm(CardIter* pCI);
   ~XF_Dedupe_colNm(void) noexcept(false);

    static void vXF_Dedupe_colNm(CardIter* pCI);

    void vXF_Dedupe_colNm(void);

  private:
    Footer* aFooter(void);
  };

  XF_Dedupe_colNm* _pXF_Dedupe_colNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
