/*============================================================================*/
/* Scanner.hpp -- Token Scanner                                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Scanner : public Object
  {
  private:
    TokenVec*     m_pTknVec;        // tokenised for of the pipeline statement
    i8            m_i_Tkn;          // current token
    Token*        m_p_Tkn;          // current token
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Scanner(Stz* szStatement);
   ~Scanner(void) noexcept(false);

    Token*     pTkn(void);
    Token*     pTkn(i8 i_Tkn);
    TokenVec*  pTknVec(void);

    void vTokenAccept(void);
    void vTokenBegin(i8 i_Tkn);

    Footer* aFooter(void);
  };

  Scanner* _pScanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
