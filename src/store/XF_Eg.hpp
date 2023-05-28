/*============================================================================*/
/* XF_Eg.hpp -- Transform Example                                             */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Eg : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Eg(void);
   ~XF_Eg(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  XF_Eg* _pXF_Eg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
