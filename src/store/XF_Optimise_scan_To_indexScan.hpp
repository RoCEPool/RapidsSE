/*============================================================================*/
/* XF_Optimise_scan_To_indexScan.hpp -- Morph scan to indexScan                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_scan_To_indexScan : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_scan_To_indexScan(CardIter* pCI);
   ~XF_Optimise_scan_To_indexScan(void) noexcept(false);

    static void vXF_Optimise_scan_To_indexScan(CardIter* pCI);

    void vXF_Replace_scan_By_indexScan(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_scan_To_indexScan* _pXF_Optimise_scan_To_indexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
