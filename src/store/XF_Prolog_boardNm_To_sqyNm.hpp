/*============================================================================*/
/* XF_Prolog_boardNm_To_sqyNm.hpp -- Change boardNm to sqyNm                  */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_boardNm_To_sqyNm : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_boardNm_To_sqyNm(CardIter* pCI);
   ~XF_Prolog_boardNm_To_sqyNm(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vProlog(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_boardNm_To_sqyNm* _pXF_Prolog_boardNm_To_sqyNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
