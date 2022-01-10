/*============================================================================*/
/* Card_Aggreg.hpp -- Aggreg                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Project.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_Aggreg : public Card_Project
  {  
  protected:

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Aggreg(eCK eKind, Stz* szLevel);
   ~Card_Aggreg(void) noexcept(false);


  private:
    static void vDeleteThis_(Card* pCard);

    
    Footer* aFooter(void);
  };

  Card_Aggreg* _pCard_Aggreg(void* aAddr);
  bool _bIsCard_Aggreg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
