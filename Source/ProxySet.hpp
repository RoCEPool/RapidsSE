/*============================================================================*/
/* ProxySet.hpp - Proxy Set                                                   */
/*============================================================================*/

#pragma once

#include "GridModel.h"

namespace GridModel
{
  class ProxySet : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/