/*============================================================================*/
/* XF_Prolog_Ensure_colType.hpp -- Ensure Column Type                         */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Ensure_colType : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Ensure_colType(CardIter* pCI);
   ~XF_Prolog_Ensure_colType(void) noexcept(false);

    static void vEnsure(CardIter* pCI);

    void vEnsure(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Ensure_colType* _pXF_Prolog_Ensure_colType(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
