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
Ribbon::Ribbon(CardCtl* pCardCtl, Object* pSurface, i8 nMaxLanesAcross)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_pLaneTabs = new IntSet(C_RIBBON_MAX_LANES);
  vInitTabs(0, C_RIBBON_LANE_WIDTH + C_RIBBON_LANE_GAP);

  m_pCardCtl = pCardCtl;
  m_pSurface = pSurface;

  m_nMaxLanesAcross = nMaxLanesAcross;
  m_jRibbonWidth = C_UNKNOWN;

  m_nTitleLines = 0;

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
/* get ctl                                                                    */
/*----------------------------------------------------------------------------*/
CardCtl* Ribbon::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_pCardCtl;
}

/*----------------------------------------------------------------------------*/
/* draw horizontal line using given char                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vDrawH(char cChr, bool bTop)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pDrawnLine = new RibbonLine;
  
  i8 nTimes = nRibbonWidth(0);

  pDrawnLine->vPut(0,cChr,nTimes);

  if (bTop) {
    vInsertBefore(pDrawnLine, pHead());
    m_nTitleLines++;
  }
  else {
    vAppend(pDrawnLine);
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
    vPutOther(i_Line,i_Char,szBar);
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
void Ribbon::vInsertCharsV(BASE::i8 i_FromLine, BASE::i8 i_Char, char cChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();
  for (i8 i_Line = i_FromLine; i_Line < nLines; i_Line++) {
    RibbonLine* pRibbonLine = pGet(i_Line);
    pRibbonLine->vInsert(i_Char,cChar);
  }
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
/* pin sub-ribbon ribbon onto this ribbon                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vPinSubRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  pSubRibbon->vTrim();
  
  i8 nLines = pSubRibbon->nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pSubLine = pSubRibbon->pGet(i_RibbonLine);
    
    Stz* szSubText = pSubLine->szText();
    vPutOther(i_Line++,i_Char,szSubText);
  }
}

/*----------------------------------------------------------------------------*/
/* print all the lines of this ribbon                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vPrintRibbon(i8 iBenchNo, i8 iPrintNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vTrim();

  vTrace("\n"_Stz);

  i8 nLines = nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pRibbonLine = pGet(i_RibbonLine);
    
    Stz* szText = pRibbonLine->szText();
    if (C_UNKNOWN != iPrintNo)

      vTrace("%llx %2llx %2llx %s\n"_Stz,iBenchNo,iPrintNo,i_RibbonLine+1,szText);
    else
      vTrace("%s\n"_Stz,szText);
  }
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprintData(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zData[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zData, xArgs, (char*)szFormat);
  va_end(xArgs);

  vPutData(m_i_Line++,m_i_Char,(Stz*)zData);
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprintOther(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zOther[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zOther, xArgs, (char*)szFormat);
  va_end(xArgs);

  vPutOther(m_i_Line++,m_i_Char,(Stz*)zOther);
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprintDataList(StzList* pStzList, i8 nWidth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  s256 sData;

  StzIter* pStzIter = pStzList->pNewIter();

  while (pStzIter->bHasNext()) {

    Stz *szText = pStzIter->pNext();

    if (0 == sData.nLength()) {
      sData = szText;
      continue;
    }
    else if (sData.nLength() + 1 + szText->nLength() > nWidth) {
      vPutData(m_i_Line++, m_i_Char, sData.szStz());
      sData = szText;
      continue;
    }
    else
      sData.vCatSprint(" %s"_Stz, szText);
  }

  if (sData.nLength()) {
    vPutData(m_i_Line++, m_i_Char, sData.szStz());
    sData = "";
  }


  delete pStzIter;
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprintDataRight(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zData[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zData, xArgs, (char*)szFormat);
  va_end(xArgs);

  char zDataRight[1024];

  StrLib::vSprint(zDataRight, m_sRightFormat.szStz(), (Stz*)zData);

  vPutData(m_i_Line++,m_i_Char,(Stz*)zDataRight);
}

/*----------------------------------------------------------------------------*/
/* get surface                                                                */
/*----------------------------------------------------------------------------*/
Object* Ribbon::pSurface(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  return m_pSurface;
}

/*----------------------------------------------------------------------------*/
/* get number of title lines                                                  */
/*----------------------------------------------------------------------------*/
i8 Ribbon::nTitleLines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_nTitleLines;
}

/*----------------------------------------------------------------------------*/
/* set number of title lines                                                  */
/*----------------------------------------------------------------------------*/
void Ribbon::vTitleLines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_nTitleLines = m_i_Line;

  vHeatSet();
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

  m_jRibbonWidth = nMaxWidth;
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
  vAppend(pRibbonLine);
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
/* put some data                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutData(i8 i_Line, i8 i_Char, Stz* szData)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vPutText(i_Line, i_Char, szData);
  if (m_pCardCtl->pCardHistory())
    m_pCardCtl->pCardHistory()->vAddLine(szData);
}

/*----------------------------------------------------------------------------*/
/* put some other                                                             */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutOther(i8 i_Line, i8 i_Char, Stz* szOther)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vPutText(i_Line, i_Char, szOther);
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
