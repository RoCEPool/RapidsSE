/*============================================================================*/
/* SEitfList.hpp - SEitf List                                                 */
/*============================================================================*/

#pragma once

#include "Interface.h"

namespace Interface {
    
  class SEitfList : public Object,
                    public List_t<SEitf>
  {
  private:
    
  public:
    void* operator new(size_t, SEitfList* pSEitfList);
    void  operator delete(void*);
    
    SEitfList(void);
    ~SEitfList(void);
    
    SEitf* pAllocSEitf(void);
    void   vFreeSEitf(SEitf* pSEitf);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/