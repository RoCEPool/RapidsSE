/*============================================================================*/
/* Ribbon.hpp -- Ribbon                                                       */
/*============================================================================*/
#pragma once

#include "Object.hpp"

#include "RibbonLine.hpp"
#include "StzList.hpp"
#include "CardCtl.hpp"

namespace BASE
{
  static const i8 C_RIBBON_ACROSS = 14;

  class Ribbon : public ListExo_t<CN_RIBBON,RibbonLine>
  {
  private:

    CardCtl* m_pCardCtl;
    Object* m_pSurface;

    i8 m_nMaxLanesAcross;  // maximum number of lanes across

    i8 m_nTitleLines;      // number of title lines
    i8 m_jRibbonWidth;     // width of ribbon

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
    
    Ribbon(CardCtl* pCardCtl, Object* pSurface, i8 nMaxLanesAcross=C_RIBBON_ACROSS);
   ~Ribbon(void) noexcept(false);

    i8   i_Char(void);
    Object* pCtl(void);

    void vDrawH(char cChr, bool bTop=false);
    void vDrawV(i8 i_Line, i8 i_Char, Stz* szBar);

    i8   i_Heat(void);
    void vHeatSet(void);
    void vHeatNext(void);

    void vInsertCharsV(i8 i_Line, i8 i_Char, char cChar=' ');
    void vInsertCharsV(i8 i_Line, i8 i_Char, Stz* szText="  "_Stz);

    void vLaneFirst(void);
    void vLaneNext(void);

    void vPinSubRibbon(i8 i_Line, i8 i_Char, Ribbon* pSubRibbon);
    void vPrintRibbon(i8 iPrintNo=C_UNKNOWN);
    void vPutText(i8 i_Line, i8 i_Char, Stz* szText);
    void vPutData(i8 i_Line, i8 i_Char, Stz* szData);
    void vPutOther(i8 i_Line, i8 i_Char, Stz* szOther);
    void vPutText2(i8 i_Line, i8 i_Char, Stz* szText2);

    void vSprint(Stz* szFormat, ...);
    void vSprintList(StzList* pStzList, i8 nWidth);
    void vSprintRight(Stz* szFormat, ...);
    Object* pSurface(void);

    i8   nTitleLines(void);
    void vTitleLines(void);
    void vTrim(void);

    i8   nRibbonWidth(i8 i_FromLine);

  private:
    void vAddLine(void);
    void vInitTabs(i8 i_Start, i8 nEach);
    void vSufficientLines(i8 i_Line);

    Footer* aFooter(void);
  };

  Ribbon* _pRibbon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
