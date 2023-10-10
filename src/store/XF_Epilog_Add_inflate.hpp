/*============================================================================*/
/* XF_Epilog_Add_inflate.hpp -- Add /<inflate>                                */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Add_inflate : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Add_inflate(CardIter* pCI);
   ~XF_Epilog_Add_inflate(void) noexcept(false);

    static void vBifurcate(CardIter* pCI);

    void vBifurcate_inflate(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Add_inflate* _pXF_Epilog_Add_inflate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
