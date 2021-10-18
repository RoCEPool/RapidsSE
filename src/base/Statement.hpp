/*============================================================================*/
/* Statement.hpp -- medium text                                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Statement : public Object
  {
  private:
    
    char  m_caText[C_SIZE_MEDIUM_TEXT];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    Statement(void);
    Statement(Stz* szText);
   ~Statement(void) noexcept(false);
    
    Stz* szText(void);
    
    Footer* aFooter(void);
  };
  
  Statement* _pStatement(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
