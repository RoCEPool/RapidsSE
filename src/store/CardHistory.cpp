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
/* return new image                                                           */
/*----------------------------------------------------------------------------*/
CardImage* CardHistory::pImage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  CardImage* pImage =
      aAdd<CardImage*>(m_pHistoryPds->pAppendWithSynthKey(),sizeof(SkipListBlock));

  return pImage;
}

/*----------------------------------------------------------------------------*/
/* return given image                                                         */
/*----------------------------------------------------------------------------*/
CardImage* CardHistory::pImage(i8 i_Image)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  CardImage* pImage =
      aAdd<CardImage*>(m_pHistoryPds->pFind(i_Image),sizeof(SkipListBlock));

  return pImage;
}

/*----------------------------------------------------------------------------*/
/* return number of images                                                    */
/*----------------------------------------------------------------------------*/
i8 CardHistory::nImages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  i8 nImages = m_pHistoryPds->nItems();

  return nImages;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
