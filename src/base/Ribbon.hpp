/*============================================================================*/
/* Ribbon.hpp -- Ribbon                                                       */
/*============================================================================*/
#pragma once

#include "Object.hpp"

#include "RibbonLine.hpp"
#include "StzList.hpp"
#include "eOH.h"

namespace BASE
{
  static const i8 C_RIBBON_ACROSS = 14;

  class Ribbon : public ListExo_t<CN_RIBBON,RibbonLine>
  {
  private:

    s32   m_sMargin;

    i8 m_nMaxLanesAcross;  // maximum number of lanes across

    i8 m_i_Heat; // current horizontal heat/row
    i8 m_i_Lane; // current vertical lane/column
    i8 m_i_Line; // current ribbon line
    i8 m_i_Char; // current ribbon char

    IntSet* m_pLaneTabs; // lane tabs

    s32 m_sRightFormat; // format for vSprintRight() based on C_RIBBON_LANE_WIDTH

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Ribbon(i8 nMaxLanesAcross=C_RIBBON_ACROSS);
   ~Ribbon(void) noexcept(false);

    i8   i_Char(void);

    void vDrawH(char cChr, i8 i_Line=C_UNKNOWN);
    void vDrawV(i8 i_Line, i8 i_Char, Stz* szBar);

    i8   i_Heat(void);
    void vHeatSet(void);
    void vHeatNext(void);

    void vInsertCharsV(i8 i_Line, i8 i_Char, Stz* szText="  "_Stz);

    void vLaneFirst(void);
    void vLaneNext(void);

    Stz* szMargin(void);
    void vMargin(Stz* szMargin);

    void vNewLine(void);

    void vPinSubRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon);
    void vPrintRibbon(Stz* szQueryNm, i8 BenchNo, eOH ePhase, i8 iPrintNo=C_UNKNOWN);
    void vPutText(i8 i_Line, i8 i_Char, Stz* szText2);

    void vSprint(i8& ri_Char, Stz* szFormat, ...);

    void vTrim(void);

    i8   nRibbonWidth(i8 i_FromLine);

  private:
    void vAddLine(void);

    void vFormatLeft(String* pString, Stz* szFormat, va_list xArgs);
    void vFormatRight(String* pString, Stz* szFormat, va_list xArgs);

    void vInitTabs(i8 i_Start, i8 nEach);

    void vSufficientLines(i8 i_Line);

    Footer* aFooter(void);
  };

  Ribbon* _pRibbon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
