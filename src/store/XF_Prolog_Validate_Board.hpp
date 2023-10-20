/*============================================================================*/
/* XF_Prolog_Validate_Board.hpp -- Validate Board                             */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Validate_Board : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Validate_Board(CardIter* pCI);
   ~XF_Prolog_Validate_Board(void) noexcept(false);

    static void vValidate(CardIter* pCI);

    void vValidate(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Validate_Board* _pXF_Prolog_Validate_Board(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
