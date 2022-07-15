/*============================================================================*/
/* TextMedium.hpp -- medium text                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class TextMedium : public TextAbstract
  {
  private:
    
    char  m_caText[C_TEXT_MEDIUM];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextMedium(void);
    TextMedium(Stz* szText);
   ~TextMedium(void) noexcept(false);
    
    static void vRepair(Object* pObject);

    Stz* szArea(void);
    
    Footer* aFooter(void);
  };
  
  TextMedium* _pTextMedium(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
