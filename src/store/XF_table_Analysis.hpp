/*============================================================================*/
/* XF_table_Analysis.hpp -- Table Analysis                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_table_Analysis : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_table_Analysis(CardIter* pCI);
   ~XF_table_Analysis(void) noexcept(false);

    static void vXF_table_Analysis(CardIter* pCI);

    void vXF_table_Analysis(void);

  private:
    Footer* aFooter(void);
  };

  XF_table_Analysis* _pXF_table_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
