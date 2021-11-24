/*============================================================================*/
/* Ribbon.hpp -- Ribbon                                                       */
/*============================================================================*/
#pragma once

#include "RibbonLine.hpp"
#include "eFR.h"

namespace BASE
{
  static const i8 C_RIBBON_ACROSS = 10;

  class Ribbon : public ListExo_t<CN_RIBBON,RibbonLine>
  {
  private:
    i8 m_iArea;  // which subject area is this (sub)ribbon?
  
    i8 m_i_Base; // base line for physical columns
    i8 m_i_Line; // current working line
    i8 m_i_Char; // current working char

    i8 m_i_Column_logical;  // logical column number
    i8 m_i_Column_physical; // physical column number

    i8 m_nTitleLines;  // number of title lines
    i8 m_nMaxAcross;   // maximum number of physical columns across
    IntSet* m_pTabSet; // column tabs

    s32 m_saFmt[FR_ENUMS];
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Ribbon(i8 iArea, i8 nMaxAcross=C_RIBBON_ACROSS);
   ~Ribbon(void) noexcept(false);

    i8   iArea(void);
  
    i8   iBase(void);
    void vBase(i8 iBase);

    void vDrawH(char cChr);
    void vDrawV(i8 i_Line, i8 i_Char, Stz* szBar);

    void vFirstColumn(void);

    Stz* szFmt(eFR e_Fmt);
    void vFmt(eFR e_Fmt, Stz* szFmt);
    void vFmtDefaults(void);

    i8   iLogicalColumn(void);
  
    void vNextColumn(void);
    void vNextLine(void);

    void vPinRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon);
    void vPrintRibbon(void);

    void vSetBase(void);
    void vSetBase(i8 i_Base);
    void vSprint(Stz* szFormat, ...);
    void vSprint(i8 i_Fmt, i8 i_RightJustified, ...);
    Stz* szText(i8 i_Line);
    i8   nTitleLines(void);
    void vTitleLines(i8 nTitleLines);
    void vTrim(void);

    i8   nWidth(i8 i_FromLine);

  private:
    void vAddLine(void);
    void vCheckLines(i8 i_Line);
    void vInitTabs(i8 i_Start, i8 nEach);
    void vPutText(i8 i_Line, i8 i_Char, Stz* szText);

    Footer* aFooter(void);
  };

  Ribbon* _pRibbon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
