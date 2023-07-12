/*============================================================================*/
/* CardImage.cpp -- Card Image                                                */
/*============================================================================*/

#include "CardImage.hpp"
#include "Card.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardImage,CN_CARDIMAGE)
__aNewFunction(CardImage,CN_CARDIMAGE)
__vDeleteFunction(CardImage,CN_CARDIMAGE)
__aFooterFunction(CardImage)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardImage::CardImage(Card* pCard)
: Object(CN_CARDIMAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  m_pCard = pCard;
  m_i_Line = 0;

  MemLib::vZero(m_xaCardLines,sizeof(m_xaCardLines),CN_CARDIMAGE);

  _checkInitialised_(CN_CARDIMAGE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardImage::~CardImage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add line                                                                   */
/*----------------------------------------------------------------------------*/
void CardImage::vAddLine(Stz* szLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  StrLib::vCopy(m_xaCardLines[m_i_Line++].m_cCardLine,szLine->azChar());
}

/*----------------------------------------------------------------------------*/
/* return given line                                                          */
/*----------------------------------------------------------------------------*/
Stz* CardImage::szLine(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  if (i_Line >= m_i_Line)
    return nullptr;

  char* azLine = m_xaCardLines[i_Line].m_cCardLine;
  return reinterpret_cast<Stz*>(azLine);
}

/*----------------------------------------------------------------------------*/
/* return number of lines                                                     */
/*----------------------------------------------------------------------------*/
i8 CardImage::nLines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  return m_i_Line;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
