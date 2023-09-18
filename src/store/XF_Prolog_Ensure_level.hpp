/*============================================================================*/
/* XF_Prolog_Ensure_level.hpp -- Ensure level                                  */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Ensure_level : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Ensure_level(CardIter* pCI);
   ~XF_Prolog_Ensure_level(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vEnsure(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Ensure_level* _pXF_Prolog_Ensure_level(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
