/*============================================================================*/
/* CardHistory.cpp -- Card History                                            */
/*============================================================================*/

#include "CardHistory.hpp"
#include "Card.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardHistory,CN_CARDHISTORY)
__aNewFunction(CardHistory,CN_CARDHISTORY)
__vDeleteFunction(CardHistory,CN_CARDHISTORY)
__aFooterFunction(CardHistory)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardHistory::CardHistory(void)
: Object(CN_CARDHISTORY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  m_pHistoryPds = new BASE::Pds(sizeof(_CardHistory));

  m_pImage_curr = nullptr;

  _checkInitialised_(CN_CARDHISTORY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardHistory::~CardHistory(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  delete m_pHistoryPds;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add new image                                                              */
/*----------------------------------------------------------------------------*/
void CardHistory::vAddImage(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

//  CardImage* pImage =
//      aAdd<CardImage*>(m_pHistoryPds->pAppendWithSynthKey(),sizeof(SkipListBlock));

  SkipListBlock* pSLB = m_pHistoryPds->pAppendWithSynthKey();

  _CardHistory* aCardHistory = static_cast<_CardHistory*>(pSLB->aItemKey());

  CardImage* pImage = new CardImage(pCard);
  aCardHistory->m_pImage = pImage;

  m_pImage_curr = aCardHistory->m_pImage;
}

/*----------------------------------------------------------------------------*/
/* add new image                                                              */
/*----------------------------------------------------------------------------*/
void CardHistory::vAddLine(Stz* szLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  m_pImage_curr->vAddLine(szLine);
}


/*----------------------------------------------------------------------------*/
/* return given image                                                         */
/*----------------------------------------------------------------------------*/
CardImage* CardHistory::pImage(i8 i_Image)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  SkipListBlock* pSLB = m_pHistoryPds->pFind(i_Image);

  _CardHistory* aCardHistory = static_cast<_CardHistory*>(pSLB->aItemKey());

  CardImage* pImage = aCardHistory->m_pImage;

  return pImage;
}

/*----------------------------------------------------------------------------*/
/* return number of images                                                    */
/*----------------------------------------------------------------------------*/
i8 CardHistory::nImages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  i8 nImages = m_pHistoryPds->nItems();

  return nImages;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
