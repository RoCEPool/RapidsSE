/*============================================================================*/
/* XF_Prolog_X_scan_To_from.hpp -- Scan Becomes Table                            */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_scan_To_from : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_scan_To_from(CardIter* pCI);
   ~XF_Prolog_X_scan_To_from(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_scan_To_table(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_X_scan_To_from* _pXF_Prolog_X_scan_To_from(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
