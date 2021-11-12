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
Ribbon::Ribbon(i8 iArea, i8 nMaxAcross)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  m_iArea = iArea;
  
  m_pTabSet = new IntSet(C_RIBBON_COLUMNS);
  vInitTabs(0,C_RIBBON_WIDTH + C_RIBBON_GAP);

  m_i_Column_logical  = 0;
  m_i_Column_physical = 0;

  m_i_Base = 0;
  m_i_Line = 0;
  m_i_Char = m_pTabSet->iInt(m_i_Column_physical) ;

  m_nTitleLines = 0;
  m_nMaxAcross  = nMaxAcross;

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
  
  delete m_pTabSet;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get subject area of this (sub)ribbon                                       */
/*----------------------------------------------------------------------------*/
i8 Ribbon::iArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  return m_iArea;
}

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
void Ribbon::vBase(i8 iBase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Base = iBase;
}

/*----------------------------------------------------------------------------*/
/* draw horizontal line using given char                                      */
/*----------------------------------------------------------------------------*/
void Ribbon::vDrawH(char cChr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  RibbonLine* pDrawnLine = new RibbonLine;
  
  i8 nTimes = nWidth(0);

  pDrawnLine->vPut(0,cChr,nTimes);
  
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
void Ribbon::vFirstColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Column_logical  = 0;
  m_i_Column_physical = 0;
  m_i_Char = m_pTabSet->iInt(m_i_Column_physical);
  
  m_i_Line = m_i_Base;
  
  vPutHeader();
}

/*----------------------------------------------------------------------------*/
/* advance to next column                                                     */
/*----------------------------------------------------------------------------*/
void Ribbon::vNextColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  if (m_i_Column_logical + 1 > C_RIBBON_COLUMNS)
    return;
    
  m_i_Column_logical++;
  
  if (++m_i_Column_physical >= m_nMaxAcross) {
    m_i_Base = nItems() + 1;
    m_i_Column_physical = 0;
  }
  
  m_i_Line = m_i_Base;
  m_i_Char = m_pTabSet->iInt(m_i_Column_physical);
    
  vPutHeader();
}
/*----------------------------------------------------------------------------*/
//void Ribbon::vNextColumn(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
//
//  if (m_i_Column_logical + 1 > C_RIBBON_COLUMNS)
//    return;
//
//  if (m_i_Char <= 40) {
//    m_i_Column_logical++;
//    m_i_Column_physical++;
//    m_i_Char = m_pTabSet->iInt(m_i_Column_physical);
//
//    m_i_Line = m_i_Base;
//  }
//  else {
//    m_i_Column_logical++;
//    m_i_Column_physical = 0;
//    m_i_Char = m_pTabSet->iInt(m_i_Column_physical);
//
//    m_i_Base = nItems() + 1;
//    m_i_Line = m_i_Base;
//  }
//
//  vPutHeader();
//}

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
void Ribbon::vPinRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon)
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
  
  vDrawH('=');
  
  vTrim();

  i8 nLines = nItems();
  for (i8 i_RibbonLine = 0; i_RibbonLine < nLines; i_RibbonLine++) {
    RibbonLine* pRibbonLine = pGet(i_RibbonLine);
    
    Stz* szText = pRibbonLine->szText();
    vTrace("%s\n"_Stz,szText);
  }
}

/*----------------------------------------------------------------------------*/
/* set base line for physical columns                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vSetBase(i8 i_Base)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Base = i_Base;
}

/*----------------------------------------------------------------------------*/
/* set base line for physical columns                                         */
/*----------------------------------------------------------------------------*/
void Ribbon::vSetBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_i_Base = m_i_Line;
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

  vPutText(m_i_Line++,m_i_Char,(Stz*)zText);
}

/*----------------------------------------------------------------------------*/
/* test next column                                                           */
/*----------------------------------------------------------------------------*/
void Ribbon::vTestNextColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  if (m_i_Line != m_i_Base)
    vNextColumn();
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
void Ribbon::vTitleLines(i8 nTitleLines)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  m_nTitleLines = nTitleLines;
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
/* put header                                                                 */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutHeader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);
  
  s32 sHeader;
  
  sHeader.vSprint("-------%02lld-------"_Stz,m_i_Column_logical);
  
  //vSprint("%s"_Stz,sHeader.szStz());
  vPutText(m_i_Line++,m_i_Char,sHeader.szStz());
}

/*----------------------------------------------------------------------------*/
/* put some text                                                              */
/*----------------------------------------------------------------------------*/
void Ribbon::vPutText(i8 i_Line, i8 i_Char, Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RIBBON,_FOOTER_);

  i8 nLines = nItems();
  
  vCheckLines(i_Line);
  
  RibbonLine* pRibbonLine = pGet(i_Line);

  pRibbonLine->vPut(i_Char,szText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
