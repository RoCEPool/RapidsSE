/*============================================================================*/
/* CN_XF_Epilog_Add_synthesis.hpp -- Add synthesis information                   */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Append_synthesis : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Append_synthesis(CardIter* pCI);
   ~XF_Epilog_Append_synthesis(void) noexcept(false);

    static void vAppend(CardIter* pCI);

    void vAppend(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Append_synthesis* _pXF_Epilog_Append_synthesis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
