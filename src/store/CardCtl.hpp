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
    bool m_bHeatNext;        // print card on new horizontal heat/row
    s32  m_sHideHeat;        // hide this heat (when it starts with this card)

    bool m_bPrintBelow;      // force print of subcards below this one
    bool m_bPrintByKind;     // print card by kind (or the following settings...)
    bool m_bPrintName;       // print name of card
    bool m_bPrintNo;         // print no of card
    bool m_bPrintValue;      // print value of card
    bool m_bPrintProperties; // print properties of card
    bool m_bPrintPptName;    // print name of properties for card?
    bool m_bPrintSubCards;   // print the subcards of this card?
    bool m_bPrintSubShort;   //   or compressed print of subcards of this card?

    bool m_bPrune;           // prune sub-card printing at this point
  };

  class CardCtl : public Object
  {
  protected:
    b8 m_bCued;
    b8 m_bCueDisabled;
    b8 m_bTrace;

    _CardCtl m_xaCardCtl[CK_ENUMS];

    _CardCtl m_xaCardCtl_default[CK_ENUMS];
    _CardCtl m_xaCardCtl_expanded[CK_ENUMS];
    _CardCtl m_xaCardCtl_longer[CK_ENUMS];
    _CardCtl m_xaCardCtl_shorter[CK_ENUMS];
    _CardCtl m_xaCardCtl_xform[CK_ENUMS];
    _CardCtl m_xaCardCtl_xhand[CK_ENUMS];

    _CardCtl m_xaCardCtl_save[CK_ENUMS];

    i8  m_iPrintNo;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardCtl(void);
   ~CardCtl(void) noexcept(false);

    bool bBenchPrint(void);
    bool bBoardPrint(void);

    bool bCued(void);
    void vCued(bool bCued);
    bool bCueDisabled(void);
    void vCueDisabled(bool bCueDisabled);
    bool bCueUser(Stz* szXhand, Stz* szXform);

    void vFormPrompt(String* sPrompt,
                     char cPrefix, Stz* szXhand, Stz* szXform,
                     i8 i_Board, i8 nBpards, i8 iLineNo);

    bool bHeatNext(i8 i_Card);
    void vHeatNext(i8 i_Card, bool bNextHoriz);

    bool bPrintBelow(i8 i_Card);
    void vPrintBelow(i8 i_Card, bool bPrintBelow);
    void vPrintBelow(i8 iBelow, va_list xVarArgs);
    bool bPrintByKind(i8 i_Card);
    void vPrintByKind(i8 i_Card, bool bPrintByKind);
    Stz* szHorizExclude(i8 i_Card);
    void vHorizExclude(i8 i_Card, Stz* szHorizExclude);
    bool bPrintName(i8 i_Card);
    void vPrintName(i8 i_Card, bool bPrintName);
    bool bPrintNo(i8 i_Card);
    void vPrintNo(i8 i_Card, bool bPrintNo);
    i8   iPrintNo(void);
    void vPrintNoInc(void);

    bool bPrintValue(i8 i_Card);
    void vPrintValue(i8 i_Card, bool bPrintValue);
    bool bPrintPptName(i8 i_Card);
    void vPrintPptName(i8 i_Card, bool bPrintProperties);
    bool bPrintProperties(i8 i_Card);
    void vPrintProperties(i8 i_Card, bool bPrintProperties);
    bool bPrintSubCards(i8 i_Card);
    void vPrintSubCards(i8 i_Card, bool bPrintSubCards);
    void vPrintSubCards(bool bPrintSubCards, ...);
    bool bPrintSubShort(i8 i_Card);
    void vPrintSubShort(i8 i_Card, bool bPrintSubShort);

    bool bPrune(i8 i_Card);
    void vPrune(i8 i_Card, bool bPrune);

    void vReset(void);
    void vRestore(void);

    void vSave(void);
    void vSaveDefault(void);
    void vSaveExpanded(void);
    void vSaveLonger(void);
    void vSaveShorter(void);
    void vSaveXform(void);
    void vSaveXhand(void);

    void vSetSubsPrintAll(void);
    void vSetSubsPrintEverything(void);
    void vSetSubsPrintNone(void);

    void vShowSettings(i8 i_Card);

    bool bTrace(void);
    void vTraceing(bool bTrace);
    bool bTracing(Stz* szXhand, Stz* szXform);

    void vUseDefault(void);
    void vUseExpanded(void);
    void vUseLonger(void);
    void vUseShorter(void);
    void vUseXform(void);
    void vUseXhand(void);

  protected:
    Footer* aFooter(void);
  };

  CardCtl* _pCardCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
