/*============================================================================*/
/* RibbonLine.hpp -- Ribbon Line                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_RIBBON_MAX_LANES  = 64;
  static const i8 C_RIBBON_LANE_WIDTH = 20;
  static const i8 C_RIBBON_LANE_GAP   = 1;

  static const i8 C_RIBBON_MAX_TEXT   = 1 + (C_RIBBON_MAX_LANES
                                              * (C_RIBBON_LANE_WIDTH + C_RIBBON_LANE_GAP));

  class RibbonLine : public Object
  {
  private:
    
    char  m_caText[C_RIBBON_MAX_TEXT];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pRibbonLine);
    
    RibbonLine(void);
   ~RibbonLine(void) noexcept(false);

    void vInsert(i8 i_Char, char cChr);
    void vInsert(i8 i_Char, Stz* szText);

    void vPut(i8 i_Char, char cChr);
    void vPut(i8 i_Char, Stz* szStz);
    void vPut(i8 i_Char, char cChr, i8 nTimes);
  
    void vSprint(i8 i_Char, Stz* szFormat, ...);
  
    Stz* szText(void);
    void vTrim(void);

    Footer* aFooter(void);
  };
  
  RibbonLine* _pRibbonLine(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
