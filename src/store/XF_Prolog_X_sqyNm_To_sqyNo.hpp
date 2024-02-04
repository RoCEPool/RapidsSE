/*============================================================================*/
/* XF_Prolog_X_sqyNm_To_sqyNo.hpp -- Change pipeNm to sqyNo                    */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_sqyNm_To_sqyNo : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_sqyNm_To_sqyNo(CardIter* pCI);
   ~XF_Prolog_X_sqyNm_To_sqyNo(void) noexcept(false);

    static void vConvert_deck_v2(CardIter* pCI);

    void vX_Board_sqyNm_To_sqyNo(void);
    void vX_Deck_sqyNm_To_sqyNo_v2(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_X_sqyNm_To_sqyNo* _pXF_Prolog_X_sqyNm_To_sqyNo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
