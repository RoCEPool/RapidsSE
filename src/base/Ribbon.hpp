/*============================================================================*/
/* Ribbon.hpp -- Ribbon                                                       */
/*============================================================================*/
#pragma once

#include "Object.hpp"

#include "RibbonLine.hpp"
#include "eFR.h"

namespace BASE
{
  static const i8 C_RIBBON_ACROSS = 10;

  class Ribbon : public ListExo_t<CN_RIBBON,RibbonLine>
  {
  private:

    Object* m_pCtl;
    Object* m_pSurface;

    i8 m_nTitleLines;  // number of title lines
    i8 m_nMaxAcross;   // maximum number of physical columns across

    i8 m_i_Base; // base line for physical columns
    i8 m_i_Lane; // current logical lane/column
    i8 m_i_Line; // current working line
    i8 m_i_Char; // current working char

    IntSet* m_pTabSet; // column tabs

    s32 m_saFmt[FR_ENUMS];

    s32 m_sRightFormat;

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Ribbon(Object* pCtl, Object* pSurface, i8 nMaxAcross=C_RIBBON_ACROSS);
   ~Ribbon(void) noexcept(false);

    i8   iBase(void);
    void vBase(void);

    i8   iChar(void);
    Object* pCtl(void);

    void vDrawH(char cChr, bool bTop=false);
    void vDrawV(i8 i_Line, i8 i_Char, Stz* szBar);

    void vFirstLane(void);

    Stz* szFmt(eFR e_Fmt);
    void vFmt(eFR e_Fmt, Stz* szFmt);
    void vFmtDefaults(void);

    i8   iLine(void);

    void vNextHeat(void);
    void vNextLane(void);
    void vNextLine(void);

    void vPinSubRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon);
    void vPrintRibbon(void);

    void vSprint(Stz* szFormat, ...);
    void vSprintRight(Stz* szFormat, ...);
    Object* pSurface(void);

    Stz* szText(i8 i_Line);
    i8   nTitleLines(void);
    void vTitleLines(void);
    void vTrim(void);

    i8   nWidth(i8 i_FromLine);

  private:
    void vAddLine(void);
    void vCheckLines(i8 i_Line);
    void vInitTabs(i8 i_Start, i8 nEach);
    void vPutText(i8 i_Line, i8 i_Char, Stz* szText);
    void vPutTextLeft(i8 i_Line, i8 i_Char, Stz* szText);
    void vPutTextRight(i8 i_Line, i8 i_Char, Stz* szText);

    Footer* aFooter(void);
  };

  Ribbon* _pRibbon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
