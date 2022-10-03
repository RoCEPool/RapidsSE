/*============================================================================*/
/* CardCtl.hpp -- Card Control                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"
#include "eCP.h"

namespace STORE
{
  struct _CardCtl
  {
    eCP  m_ePrintForm;      // print form
    bool m_bPrintSubCards;  // print the subcards of this card?
    bool m_bNextHoriz;      // print card on new horizontal line
  };

  class CardCtl : public Object
  {
  protected:
    _CardCtl m_xaCardCtl[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardCtl(void);
   ~CardCtl(void) noexcept(false);

    bool bNextHoriz(i8 i_Card);
    void vNextHorizon(i8 i_Card, bool bNextHoriz);

    eCP  ePrintForm(i8 i_Card);
    void vPrintForm(i8 i_Card, eCP ePrintForm);
    void vPrintFormSubCards(i8 i_Card, eCP ePrintForm, bool bPrintSubCards);

    bool bPrintSubCards(i8 i_Card);
    void vPrintSubCards(i8 i_Card, bool bPrintSubCards);

  protected:
    Footer* aFooter(void);
  };

  CardCtl* _pCardCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
