/*============================================================================*/
/* Descend.hpp -- token descender                                             */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Descend : public Object
  {
  private:
    TokenVec* m_pTokenVec;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddress);
    
    Descend(TokenVec* pArgVec);
   ~Descend(void) noexcept(false);
  
    i8     iCurr(void);
    Token* pCurr(void);
  
    Token* pFirst(void);
    void   vFirst(void);

    Token* pLast(void);
    void   vLast(void);

    void   vNext(void);
    Token* pNext(void);
  
    void   vPrev(void);
    Token* pPrev(void);

    Footer* aFooter(void);
  };

  Descend* _pDescend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
