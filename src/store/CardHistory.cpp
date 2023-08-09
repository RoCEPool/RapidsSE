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
void CardHistory::vAddImage(Card* pCard, i8 iXform, Stz* szFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

//  CardImage* pImage =
//      aAdd<CardImage*>(m_pHistoryPds->pAppendWithSynthKey(),sizeof(SkipListBlock));

  SkipListBlock* pSLB = m_pHistoryPds->pAppendWithSynthKey();

  _CardHistory* aCardHistory = static_cast<_CardHistory*>(pSLB->aItemKey());

  CardImage* pCardImage = new CardImage(pCard,pSLB->iKey(),iXform,szFunc);
  aCardHistory->m_pImage = pCardImage;

  m_pImage_curr = aCardHistory->m_pImage;

  CardImage* pTest = pImage(aCardHistory->m_iKey);

  RASSERT(pTest == pCardImage);
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
/* dump images                                                                */
/*----------------------------------------------------------------------------*/
void CardHistory::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  m_pHistoryPds->vDumpInUseList();

//  i8 nImages = m_pHistoryPds->nItems();
  for (i8 i_Image = 1; /**/; i_Image++) {
    CardImage* pCardImage = pImage(i_Image);
    if (! pCardImage)
      break;
    pCardImage->vDump(i_Tab);
  }

}


/*----------------------------------------------------------------------------*/
/* return given image                                                         */
/*----------------------------------------------------------------------------*/
CardImage* CardHistory::pImage(i8 i_Image)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  SkipListBlock* pSLB = m_pHistoryPds->pFind(i_Image);

  if (! pSLB)
    return nullptr;

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

/*----------------------------------------------------------------------------*/
/* print images                                                               */
/*----------------------------------------------------------------------------*/
void CardHistory::vPrintImages(eCK eCard, eAL eLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDHISTORY,_FOOTER_);

  i8 nMaxLines = 0;
  for (i8 i_Image = 1; /**/; i_Image++) {
    CardImage* pCardImage = pImage(i_Image);
    if (! pCardImage)
      break;
    if (pCardImage->eCard() != eCard)
      continue;
    if (AL_NONE != eLevel && pCardImage->eLevel() != eLevel)
      continue;
    i8 nLines = pCardImage->nLines();
    if (nLines > nMaxLines)
      nMaxLines = nLines;
  }

  for (i8 i_Line = 0; i_Line < nMaxLines; i_Line++) {
    for (i8 i_Image = 1; /**/; i_Image++) {
      CardImage *pCardImage = pImage(i_Image);
      if (! pCardImage)
        break;
      if (pCardImage->eCard() != eCard)
        continue;
      if (AL_NONE != eLevel && pCardImage->eLevel() != eLevel)
        continue;
      pCardImage->vPrintLine(i_Line);
    }
    vTrace("\n"_Stz);
  }

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
