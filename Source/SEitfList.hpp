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
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, SEitfList* pSEitfList);
    void  operator delete(void*);
    
    SEitfList(void);
    ~SEitfList(void);
    
    SEitf* pAllocSEitf(void);
    void   vFreeSEitf(SEitf* pSEitf);
    
    Footer* pFooter(void);
  };
    
  SEitfList* _pSEitfList(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/