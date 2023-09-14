/*============================================================================*/
/* CardImage.cpp -- Card Image                                                */
/*============================================================================*/

#include "CardImage.hpp"
#include "Card.hpp"
#include "Card_Level.hpp"

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
CardImage::CardImage(Card* pCard, i8 iSLB, i8 iXform, Stz* szXfunc)
: Object(CN_CARDIMAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  m_pCard  = pCard;
  m_eCard  = m_pCard->eCard();

  m_eLevel = AL_NONE;
  if (pCard->pSub_level())
    m_eLevel = (eAL)pCard->pSub_level()->pSymbol()->Y_iGet(P_Level);
  m_iSLB   = iSLB;
  m_iXform = iXform;
  m_sXfunc = szXfunc;

  m_i_Line = 0;

  MemLib::vZero(m_xaCardLines,sizeof(m_xaCardLines),CN_CARDIMAGE);

  _checkInitialised_(CN_CARDIMAGE);

  s32 sXinfo;
  s32 sXinfo_padded;

  sXinfo.vSprint("Img %lld XF %lld"_Stz, m_iSLB, m_iXform);
  sXinfo_padded.vSprint("%-20s"_Stz, sXinfo.szStz());
  vAddLine(sXinfo_padded);

  sXinfo.vSprint("%s"_Stz, szXfunc);
  StrLib::vRemoveTill(sXinfo.azChar(), '_', true);
  StrLib::vPut(sXinfo.azChar(), sXinfo.nAreaBytes(), 20, '\0');
  sXinfo_padded.vSprint("%-20s"_Stz, sXinfo.szStz());
  vAddLine(sXinfo_padded);

  sXinfo.vSprint("%s"_Stz, "<note>"_Stz);
  StrLib::vPut(sXinfo.azChar(), sXinfo.nAreaBytes(), 20, '\0');
  sXinfo_padded.vSprint("%-20s"_Stz, sXinfo.szStz());
  vAddLine(sXinfo_padded);

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

  i8 iBufSize = sizeof(m_xaCardLines[m_i_Line].m_zCardLine);
  StrLib::vCopy(m_xaCardLines[m_i_Line++].m_zCardLine, iBufSize, szLine->azChar());
}

/*----------------------------------------------------------------------------*/
/* return eCard                                                               */
/*----------------------------------------------------------------------------*/
eCK CardImage::eCard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDIMAGE, _FOOTER_);

  return m_eCard;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void CardImage::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  for (i8 i_Line = 0; i_Line < m_i_Line; i_Line++) {
    vTrace("%s%s\n"_Stz,
           Stz::szTab(i_Tab),
           m_xaCardLines[i_Line].m_zCardLine);
  }
}

/*----------------------------------------------------------------------------*/
/* return eLevel                                                              */
/*----------------------------------------------------------------------------*/
eAL CardImage::eLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDIMAGE, _FOOTER_);

  return m_eLevel;
}


/*----------------------------------------------------------------------------*/
/* return given line                                                          */
/*----------------------------------------------------------------------------*/
Stz* CardImage::szLine(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  if (i_Line >= m_i_Line)
    return nullptr;

  char* azLine = m_xaCardLines[i_Line].m_zCardLine;
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

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void CardImage::vPrintLine(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDIMAGE,_FOOTER_);

  if (i_Line < m_i_Line)
    vTrace("%s"_Stz,
           m_xaCardLines[i_Line].m_zCardLine);
  else
    vTrace("%20s"_Stz,"");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
