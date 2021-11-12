/*============================================================================*/
/* RibbonLine.hpp -- Ribbon Line                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_RIBBON_COLUMNS = 32;
  static const i8 C_RIBBON_WIDTH   = 16;
  static const i8 C_RIBBON_GAP     = 1;

  static const i8 C_RIBBON_TEXT = C_RIBBON_COLUMNS
                                    * (C_RIBBON_WIDTH + C_RIBBON_GAP)
                                    + 1;

  class RibbonLine : public Object
  {
  private:
    
    char  m_caText[C_RIBBON_TEXT];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pRibbonLine);
    
    RibbonLine(void);
   ~RibbonLine(void) noexcept(false);
    
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
