/*============================================================================*/
/* Card_Atom.hpp -- Atom Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Atom : public Card
  {
  protected:
    s256 m_sAtom;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Atom(Stz* szAtom);
   ~Card_Atom(void) noexcept(false);

    Stz* szAtom(void);
    void vAtom(Stz* szAtom);
  
    bool bIsAggOp(void);
    bool bIsOrdinalColumnAtom(void);
    bool bIsOrdinalIndexAtom(void);
    i8   iOrdinalColumnValue(void);
    i8   iOrdinalIndexValue(void);

   private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_Atom* _pCard_Atom(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
