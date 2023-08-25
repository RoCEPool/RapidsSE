/*============================================================================*/
/* XF_Prolog_Shorten_queryNm.hpp -- Shorten Query Names                           */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Shorten_queryNm : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Shorten_queryNm(CardIter* pCI);
   ~XF_Prolog_Shorten_queryNm(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_Shorten_queryNm(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Shorten_queryNm* _pXF_Prolog_Shorten_queryNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
