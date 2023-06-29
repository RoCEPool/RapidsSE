/*============================================================================*/
/* XF_index_Analysis.hpp -- Index Analysis                                    */
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

    void vXF_index_Analysis(void);

  private:
    Footer* aFooter(void);
  };

  XF_index_Analysis* _pXF_index_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
