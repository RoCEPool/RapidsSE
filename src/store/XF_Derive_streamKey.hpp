/*============================================================================*/
/* XF_Derive_streamKey.hpp -- Derive projection key                      */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Derive_streamKey : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Derive_streamKey(CardIter* pCI);
   ~XF_Derive_streamKey(void) noexcept(false);

    static void vXF_Derive_streamKey(CardIter* pCI);

    void vXF_Derive_streamKey(void);

  private:
    bool bFindStart_streamKey(void);
    void vPropagate_streamKey(void);


    Footer* aFooter(void);
  };

  XF_Derive_streamKey* _pXF_Derive_streamKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
