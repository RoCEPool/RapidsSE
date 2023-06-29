/*============================================================================*/
/* XF_Shorten_QueryNames.hpp -- Shorten Query Names                           */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Shorten_QueryNames : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Shorten_QueryNames(CardIter* pCI);
   ~XF_Shorten_QueryNames(void) noexcept(false);

    static void vXF_Shorten_QueryNames(CardIter* pCI);

    void vXF_Shorten_QueryNames(void);

  private:
    Footer* aFooter(void);
  };

  XF_Shorten_QueryNames* _pXF_Shorten_QueryNames(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
