/*============================================================================*/
/* XF_OT_Replace_scan_By_indexScan.hpp -- Morph scan to indexScan                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_OT_Replace_scan_By_indexScan : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_OT_Replace_scan_By_indexScan(CardIter* pCI);
   ~XF_OT_Replace_scan_By_indexScan(void) noexcept(false);

    static void vXF_Replace_scan_By_indexScan(CardIter* pCI);

    void vXF_Replace_scan_By_indexScan(void);

  private:
    Footer* aFooter(void);
  };

  XF_OT_Replace_scan_By_indexScan* _pXF_OT_Replace_scan_By_indexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
