/*============================================================================*/
/* XF_scan_Becomes_table.hpp -- Scan Becomes Table                            */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_scan_Becomes_table : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_scan_Becomes_table(CardIter* pCI);
   ~XF_scan_Becomes_table(void) noexcept(false);

    static void vXF_scan_Becomes_table(CardIter* pCI);

    void vXF_scan_Becomes_table(void);

  private:
    Footer* aFooter(void);
  };

  XF_scan_Becomes_table* _pXF_scan_Becomes_table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
