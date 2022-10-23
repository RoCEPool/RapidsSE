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
Ribbon::Ribbon(Object* pCtl, Object* pSurface, i8 nMaxAcross)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  m_pCtl = pCtl;
  m_pSurface = pSurface;
  
  m_pTabSet = new IntSet(C_RIBBON_COLUMNS);
  vInitTabs(0,C_RIBBON_WIDTH + C_RIBBON_GAP);

  m_nMaxAcross  = nMaxAcross;
  m_nTitleLines = 0;

  m_i_Base = 0;
  m_i_Lane = 0;
  m_i_Line = 0;
  m_i_Char = m_pTabSet->iInt(m_i_Lane) ;

  // m_saFmt
  
  _checkInitialised_(CN_RIBBON);
  
  vFmtDefaults();
  
  _postcondition_(_MAGIC_,CN_RIBBON,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ribbon::~Ribbon(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vDeleteAll();
  
  delete m_pTabSet;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get base line for physical columns                                         */
/*----------------------------------------------------------------------------*/
i8 Ribbon::iBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_i_Base;
}

/*----------------------------------------------------------------------------*/
/* set base line for physical columns                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Base = m_i_Line;
}

/*----------------------------------------------------------------------------*/
/* get current char                                                           */
/*----------------------------------------------------------------------------*/
i8 Ribbon::iChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_i_Char;
}

/*----------------------------------------------------------------------------*/
/* get ctl                                                                    */
/*----------------------------------------------------------------------------*/
Object* Ribbon::pCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_pCtl;
}

/*----------------------------------------------------------------------------*/
/* draw horizontal line using given char                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vDrawH(char cChr, bool bTop)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pDrawnLine = new RibbonLine;
  
  i8 nTimes = nWidth(0);

  pDrawnLine->vPut(0,cChr,nTimes);

  if (bTop)
    vInsertBefore(pDrawnLine,pHead());
  else
    vAppend(pDrawnLine);
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
/* begin at first column                                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vFirstLane(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Lane = 0;
  m_i_Char = m_pTabSet->iInt(m_i_Lane);
  
  m_i_Line = m_i_Base;
  
  //vPutHeader();
}

/*----------------------------------------------------------------------------*/
/* get a format                                                               */
/*----------------------------------------------------------------------------*/
Stz* Ribbon::szFmt(eFR i_Fmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  return m_saFmt[i_Fmt];
}

/*----------------------------------------------------------------------------*/
/* set a format                                                               */
/*----------------------------------------------------------------------------*/
void Ribbon::vFmt(eFR i_Fmt, Stz* szFmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  m_saFmt[i_Fmt] = szFmt;
}

/*----------------------------------------------------------------------------*/
/* set default formats                                                        */
/*----------------------------------------------------------------------------*/
void Ribbon::vFmtDefaults(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  m_sRightFormat = "%20s"_Stz;
  
  for (i8 i_Fmt = 0; i_Fmt < FR_ENUMS; i_Fmt++)
    vFmt((eFR)i_Fmt, aFR_format[i_Fmt]);
}

/*----------------------------------------------------------------------------*/
/* get current line                                                           */
/*----------------------------------------------------------------------------*/
i8 Ribbon::iLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_i_Line;
}

/*----------------------------------------------------------------------------*/
/* advance to next horizontal area                                            */
/*----------------------------------------------------------------------------*/
void Ribbon::vNextHeat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  m_i_Base = nItems() + 1;
  m_i_Lane = 0;
  
  m_i_Char = m_pTabSet->iInt(m_i_Lane);
  
  m_i_Line = m_i_Base;

}

/*----------------------------------------------------------------------------*/
/* advance to next column                                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vNextLane(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  if (++m_i_Lane >= m_nMaxAcross)
    vNextHeat();

  m_i_Line = m_i_Base;
  m_i_Char = m_pTabSet->iInt(m_i_Lane);
}

/*----------------------------------------------------------------------------*/
/* move to next line                                                          */
/*----------------------------------------------------------------------------*/
void Ribbon::vNextLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Line++;
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
    
    vPutText(i_Line++,i_Char,szSubText);
  }
}

/*----------------------------------------------------------------------------*/
/* print all the lines of this ribbon                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vPrintRibbon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  vDrawH('-',true);
  vDrawH('=');
  
  vTrim();

  vTrace("\n"_Stz);

  i8 nLines = nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pRibbonLine = pGet(i_RibbonLine);
    
    Stz* szText = pRibbonLine->szText();
    vTrace("%s\n"_Stz,szText);
  }
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprint(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zText[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zText,xArgs,(char*)szFormat);
  va_end(xArgs);

  vPutText(m_i_Line,m_i_Char,(Stz*)zText);
  vNextLine();
}

/*----------------------------------------------------------------------------*/
/* sprint at current coord                                                    */
/*----------------------------------------------------------------------------*/
void Ribbon::vSprintRight(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zText[1024];

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(zText,xArgs,(char*)szFormat);
  va_end(xArgs);

  vPutTextRight(m_i_Line,m_i_Char,(Stz*)zText);
  vNextLine();
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
/* get text at given line                                                     */
/*----------------------------------------------------------------------------*/
Stz* Ribbon::szText(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pRibbonLine = pGet(i_Line);
  
  Stz* szText = pRibbonLine->szText();
  
  return szText;
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
i8 Ribbon::nWidth(i8 i_FromLine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nMaxWidth = 0;
  
  i8 nLines = nItems();
  for (i8 i_Line = i_FromLine; i_Line < nLines; i_Line++) {
    RibbonLine* pRibbonLine = pGet(i_Line);
    
    Stz* szText = pRibbonLine->szText();
    i8 nWidthLine = StrLib::nLengthNonBlank(szText->azChar(),'\0',C_RIBBON_TEXT);
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
  vAppend(pRibbonLine);
}

/*----------------------------------------------------------------------------*/
/* check sufficient lines                                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vCheckLines(i8 i_Line)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();

  if (i_Line + 1 <= nLines)
    return;
  
  i8 nToAdd = (i_Line + 1) - nLines;
  while (nToAdd--)
    vAddLine();
}

/*----------------------------------------------------------------------------*/
/* set columns tabs                                                           */
/*----------------------------------------------------------------------------*/
void Ribbon::vInitTabs(i8 i_Start, i8 nEach)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 iAt = i_Start;
  for (i8 i_Col = 0; i_Col < C_RIBBON_COLUMNS; i_Col++) {
    m_pTabSet->vInt(i_Col,iAt);
    iAt += nEach;
  }
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutText(i8 i_Line, i8 i_Char, Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  vCheckLines(i_Line);
  
  RibbonLine* pRibbonLine = pGet(i_Line);

  pRibbonLine->vPut(i_Char,szText);
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutTextLeft(i8 i_Line, i8 i_Char, Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  vCheckLines(i_Line);
  
  RibbonLine* pRibbonLine = pGet(i_Line);

  pRibbonLine->vPut(i_Char,szText);
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutTextRight(i8 i_Line, i8 i_Char, Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  char zRightText[1024];

  StrLib::vSprint(zRightText,m_sRightFormat.szStz(),szText);

  vCheckLines(i_Line);
  
  RibbonLine* pRibbonLine = pGet(i_Line);

  pRibbonLine->vPut(i_Char,(Stz*)zRightText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
