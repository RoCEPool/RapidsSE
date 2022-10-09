/*============================================================================*/
/* CardCtl.hpp -- Card Control                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  struct _CardCtl
  {
    bool m_bNextHoriz;       // print card on new horizontal line
    s32  m_sHorizExclude;    // has this property for horizontal non-print

    bool m_bPrintByKind;     // print card by kind (or the following settings...)
    bool m_bPrintName;       // print name of card
    bool m_bPrintValue;      // print value of card
    bool m_bPrintProperties; // print properties of card
    bool m_bPrintPptName;    // print name of properties for card?
    bool m_bPrintSubCards;   // print the subcards of this card?
    bool m_bPrintSubShort;   //   or compressed print of subcards of this card?

    i8   m_iPtrDetail;       // ptr print detail
    bool m_bPrune;           // prune sub-card printing at this point

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

    bool bPrintByKind(i8 i_Card);
    void vPrintByKind(i8 i_Card, bool bPrintByKind);
    Stz* szHorizExclude(i8 i_Card);
    void vHorizExclude(i8 i_Card, Stz* szHorizExclude);
    bool bPrintName(i8 i_Card);
    void vPrintName(i8 i_Card, bool bPrintName);
    bool bPrintValue(i8 i_Card);
    void vPrintValue(i8 i_Card, bool bPrintValue);
    bool bPrintPptName(i8 i_Card);
    void vPrintPptName(i8 i_Card, bool bPrintProperties);
    bool bPrintProperties(i8 i_Card);
    void vPrintProperties(i8 i_Card, bool bPrintProperties);
    bool bPrintSubCards(i8 i_Card);
    void vPrintSubCards(i8 i_Card, bool bPrintSubCards);
    bool bPrintSubShort(i8 i_Card);
    void vPrintSubShort(i8 i_Card, bool bPrintSubShort);

    i8   iPtrDetail(i8 i_Card);
    void vPtrDetail(i8 i_Card, i8 iPtrDetail);

    bool bPrune(i8 i_Card);
    void vPrune(i8 i_Card, bool bPrune);

  protected:
    Footer* aFooter(void);
  };

  CardCtl* _pCardCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
