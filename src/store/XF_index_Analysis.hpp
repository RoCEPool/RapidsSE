/*============================================================================*/
/* XF_index_Analysis.hpp -- Index Prior                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_index_Analysis : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_index_Analysis(CardIter* pCI);
   ~XF_index_Analysis(void) noexcept(false);

    static void vXF_index_Analysis(CardIter* pCI);
    static void vXF_index_Analysis_passThru(CardIter* pCI);

    void vXF_index_Analysis(void);
    void vXF_index_Analysis_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_index_Analysis* _pXF_index_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
