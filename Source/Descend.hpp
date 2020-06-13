/*============================================================================*/
/* Descend.hpp -- token descender                                             */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Descend;

  struct DescendDsp
  {
    s32 m_sName;
    bool (*m_pFunc)(Descend*);
  };
}

/*----------------------------------------------------------------------------*/

namespace CompilerStream
{
  class Descend : public Object
  {
    static const i8 C_MAX_DESCENDDSP = 64;

  private:
  
    i8 m_nDispatch;
    DescendDsp m_xaDispatch[C_MAX_DESCENDDSP];
  
    i8 m_iNesting;
 
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

    void   vNestDec(void);
    void   vNestInc(void);
    i8     iNesting(void);

    void   vNext(void);
    Token* pNext(void);
  
    void   vPrev(void);
    Token* pPrev(void);
  
    //
  
    bool bDispatch(void);
    void vDispatchInit(void);
    bool bIsDispatchable(void);
    i8   iFindDispatch(Stz* szChars);
   

    Footer* aFooter(void);
  };

  Descend* _pDescend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
