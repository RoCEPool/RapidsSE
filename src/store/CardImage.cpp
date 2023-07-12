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
CardImage::CardImage(void)
: Object(CN_CARDIMAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

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
void CardImage::vAddLine(char *azLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  StrLib::vCopy(m_xaCardLines[m_i_Line++].m_cCardLine,azLine);
}

/*----------------------------------------------------------------------------*/
/* return given line                                                          */
/*----------------------------------------------------------------------------*/
char* CardImage::azLine(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  if (i_Line >= m_i_Line)
    return nullptr;

  char* azLine = m_xaCardLines[i_Line].m_cCardLine;
  return azLine;
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
