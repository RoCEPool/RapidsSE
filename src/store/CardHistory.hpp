/*============================================================================*/
/* CardHistory.hpp -- Card History                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Pds.hpp"
#include "CardImage.hpp"
#include "eCK.h"
namespace STORE
{
  class Card;

  struct _CardHistory {
    i8         m_iKey;
    CardImage* m_pImage;
  };

  class CardHistory : public Object
  {
  private:

    Pds* m_pHistoryPds;

    CardImage* m_pImage_curr;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardHistory(void);
   ~CardHistory(void) noexcept(false);

    void vAddImage(Card* pCard, i8 iXform);
    void vAddLine(Stz* szText);

    void vDump(i8 i_Tab=0);

    CardImage* pImage(i8 i_Image);
    i8 nImages(void);

    void vPrintImages(eCK eCard=CK_GBYAGG, eAL eLevel=AL_NONE);

  protected:
    Footer* aFooter(void);
  };

  CardHistory* _pCardHistory(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
