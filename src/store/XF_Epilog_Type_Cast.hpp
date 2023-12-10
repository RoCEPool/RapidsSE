/*============================================================================*/
/* XF_Epilog_Type_Cast.hpp -- Type Cast                                       */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Type_Cast : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Type_Cast(CardIter* pCI);
   ~XF_Epilog_Type_Cast(void) noexcept(false);

    static void vCast(CardIter* pCI);

    void vCast(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Type_Cast* _pXF_Epilog_Type_Cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
