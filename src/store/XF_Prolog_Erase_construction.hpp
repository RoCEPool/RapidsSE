/*============================================================================*/
/* XF_Prolog_Erase_construction.hpp -- Delete Unneeded                                  */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Erase_construction : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Erase_construction(CardIter* pCI);
   ~XF_Prolog_Erase_construction(void) noexcept(false);

    static void vErase(CardIter* pCI);

    void vProlog(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Erase_construction* _pXF_Prolog_Erase_construction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
