/*============================================================================*/
/* Ribbon.cpp -- Ribbon                                                       */
/*============================================================================*/

#include "BASE.h"

#include "Ribbon.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Ribbon,CN_RIBBON)
__aNewFunction(Ribbon,CN_RIBBON)
__vDeleteFunction(Ribbon,CN_RIBBON)
__aFooterFunction(Ribbon)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Ribbon::Ribbon(i8 nMaxLanesAcross)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_pLaneTabs = new IntSet(C_RIBBON_MAX_LANES);
  vInitTabs(0, C_RIBBON_LANE_WIDTH + C_RIBBON_LANE_GAP);

  m_nMaxLanesAcross = nMaxLanesAcross;

  m_i_Heat = 0;
  m_i_Lane = 0;
  m_i_Line = 0;
  m_i_Char = m_pLaneTabs->iInt(m_i_Lane) ;

  m_sRightFormat.vSprint("%%%llds"_Stz, C_RIBBON_LANE_WIDTH);

  _checkInitialised_(CN_RIBBON);
  
  _postcondition_(_MAGIC_,CN_RIBBON,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ribbon::~Ribbon(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vDeleteAll();
  
  delete m_pLaneTabs;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current char                                                           */
/*----------------------------------------------------------------------------*/
i8 Ribbon::i_Char(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_i_Char;
}

/*----------------------------------------------------------------------------*/
/* draw horizontal line using given char                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vDrawH(char cChr,  i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pDrawnLine = new RibbonLine;
  
  i8 nTimes = nRibbonWidth(0);

  pDrawnLine->vPut(0,cChr,nTimes);

  if (C_UNKNOWN == i_Line) {
    vAppend(pDrawnLine);
  }
//  else if (0 == i_Line) {
//    vInsertBefore(pDrawnLine, pHead());
//    m_nTitleLines++;
//  }
  else {
    vInsertBefore(pDrawnLine, pGet(i_Line));
  }
}

/*----------------------------------------------------------------------------*/
/* draw vertical bar                                                          */
/*----------------------------------------------------------------------------*/
void Ribbon::vDrawV(i8 i_FromLine, i8 i_Char, Stz* szBar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  i8 nLines = nItems();
  for (i8 i_Line = i_FromLine; i_Line < nLines; i_Line++) {
    vPutText(i_Line,i_Char,szBar);
  }
}

/*----------------------------------------------------------------------------*/
/* get base line for current heat                                             */
/*----------------------------------------------------------------------------*/
i8 Ribbon::i_Heat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_i_Heat;
}

/*----------------------------------------------------------------------------*/
/* advance to next horizontal heat                                            */
/*----------------------------------------------------------------------------*/
void Ribbon::vHeatNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Heat = nItems() + 1;
  m_i_Lane = 0;

  m_i_Line = m_i_Heat;
  m_i_Char = m_pLaneTabs->iInt(m_i_Lane);
}

/*----------------------------------------------------------------------------*/
/* set base line for current heat                                             */
/*----------------------------------------------------------------------------*/
void Ribbon::vHeatSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Heat = m_i_Line;
}

/*----------------------------------------------------------------------------*/
/* insert chars vertically                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vInsertCharsV(BASE::i8 i_FromLine, BASE::i8 i_Char, Stz* szChars)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();
  for (i8 i_Line = i_FromLine; i_Line < nLines; i_Line++) {
    RibbonLine* pRibbonLine = pGet(i_Line);
    pRibbonLine->vInsert(i_Char,szChars);
  }
}

/*----------------------------------------------------------------------------*/
/* begin at first vertical lane                                               */
/*----------------------------------------------------------------------------*/
void Ribbon::vLaneFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Lane = 0;

  m_i_Line = m_i_Heat;
  m_i_Char = m_pLaneTabs->iInt(m_i_Lane);
}

/*----------------------------------------------------------------------------*/
/* advance to next vertical lane                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vLaneNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  if (++m_i_Lane >= m_nMaxLanesAcross) {
    vHeatNext();

    m_i_Lane = 1;
  }

  m_i_Line = m_i_Heat;
  m_i_Char = m_pLaneTabs->iInt(m_i_Lane);
}

/*----------------------------------------------------------------------------*/
/* get margin                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Ribbon::szMargin(void)
{
  _precondition_(_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_sMargin;
}

/*----------------------------------------------------------------------------*/
/* set margin                                                                 */
/*----------------------------------------------------------------------------*/
void Ribbon::vMargin(Stz* szMargin)
{
  _precondition_(_MAGIC_,CN_RIBBON,_FOOTER_);

  m_sMargin= szMargin;
}

/*----------------------------------------------------------------------------*/
/* pin sub-ribbon ribbon onto this ribbon                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vPinSubRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  pSubRibbon->vTrim();

  if (*(char*)pSubRibbon->szMargin() && ! *(char*)szMargin())
    vMargin(pSubRibbon->szMargin());

  i8 nLines = pSubRibbon->nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pSubLine = pSubRibbon->pGet(i_RibbonLine);

    Stz* szSubLineMargin = pSubLine->szMargin();
    vPutText(i_Line, i_Char, szSubLineMargin);

    Stz* szSubLineText = pSubLine->szText();
    vPutText(i_Line, i_Char+5, szSubLineText);

    i_Line++;
  }
}

/*----------------------------------------------------------------------------*/
/* print all the lines of this ribbon                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vPrintRibbon(Stz* szQueryNm, i8 iBenchNo, eOH ePhase, i8 iPrintNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vTrim();

  vTrace("\n"_Stz);

  i8 nLines = nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pRibbonLine = pGet(i_RibbonLine);
    
    Stz* szText_ribbon = pRibbonLine->szText();
    if (C_UNKNOWN != iPrintNo)

      vTrace("%lld %s %2lld %s %2lld %s\n"_Stz,
             iBenchNo,
             szQueryNm,
             iPrintNo,
             aOH_short[ePhase],
             i_RibbonLine+1,
             szText_ribbon);
    else
      vTrace("%s\n"_Stz, szText_ribbon);
  }
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprint(i8& ri_Char, Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_RIBBON, _FOOTER_);

  if (C_UNKNOWN == ri_Char)
    ri_Char = m_i_Char;

  s4K sText;

  va_list xArgs;
  va_start(xArgs,szFormat);

  vFormatLeft(sText,szFormat,xArgs);
  vPutText(m_i_Line, ri_Char, sText);

  va_end(xArgs);

  ri_Char += sText.nLength();
}

/*----------------------------------------------------------------------------*/
/* trim all the lines                                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vTrim(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();
  for (i8 i_Line = 0; i_Line < nLines; i_Line++) {
    RibbonLine* pRibbonLine = pGet(i_Line);
    
    pRibbonLine->vTrim();
  }
}

/*----------------------------------------------------------------------------*/
/* calc width of ribbon                                                       */
/*----------------------------------------------------------------------------*/
i8 Ribbon::nRibbonWidth(i8 i_FromLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nMaxWidth = 0;
  
  i8 nLines = nItems();
  for (i8 i_Line = i_FromLine; i_Line < nLines; i_Line++) {
    RibbonLine* pRibbonLine = pGet(i_Line);
    
    Stz* szText = pRibbonLine->szText();
    i8 nWidthLine = StrLib::nLengthNonBlank(szText->azChar(), '\0', C_RIBBON_MAX_TEXT);
    if (nWidthLine > nMaxWidth)
      nMaxWidth = nWidthLine;
  }

  return nMaxWidth;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new ribbon line                                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vAddLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pRibbonLine = new RibbonLine;

  pRibbonLine->vMargin(szMargin());

  vAppend(pRibbonLine);
}

/*----------------------------------------------------------------------------*/
/* formatted concatenated sprint                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vFormatLeft(String* pString, Stz* szFormat, va_list args)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  StrLib::vSprint(pString->azStr(), pString->nAreaBytes(), args, (const char*)szFormat);
}

/*----------------------------------------------------------------------------*/
/* formatted concatenated sprint                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vFormatRight(String* pString, Stz* szFormat, va_list args)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  s256 sTemp;
  StrLib::vSprint(sTemp.azChar(), sTemp.nAreaBytes(), args, (const char*)szFormat);

  StrLib::vSprint(pString->azStr(), pString->nAreaBytes(), nullptr, m_sRightFormat.szStz(), sTemp.azChar());
}

/*----------------------------------------------------------------------------*/
/* set columns tabs                                                           */
/*----------------------------------------------------------------------------*/
void Ribbon::vInitTabs(i8 i_Start, i8 nEach)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 iAt = i_Start;
  for (i8 i_Col = 0; i_Col < C_RIBBON_MAX_LANES; i_Col++) {
    m_pLaneTabs->vInt(i_Col, iAt);
    iAt += nEach;
  }
}

/*----------------------------------------------------------------------------*/
/* add a new ribbon line                                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vNewLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Line++;
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutText(i8 i_Line, i8 i_Char, Stz* szText2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vSufficientLines(i_Line);

  RibbonLine* pRibbonLine = pGet(i_Line);

  pRibbonLine->vPut(i_Char, szText2);
}

/*----------------------------------------------------------------------------*/
/* check sufficient lines                                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vSufficientLines(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();

  if (i_Line + 1 <= nLines)
    return;

  i8 nToAdd = (i_Line + 1) - nLines;
  while (nToAdd--)
    vAddLine();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
