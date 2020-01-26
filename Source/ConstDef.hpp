/*============================================================================*/
/* ConstDef.hpp - Constant Definition                                         */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class ConstDef : public Object
  {
  public:
    //MemberSet* m_p_MemberSet; // the _p_ sequence means not owned by
  
    // const name
    s32 m_sConstNm;   // name of the constant
    i8  m_iValue;     // value of the constant
  
    EnameSet m_oTerms; // terms of this constant

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ConstDef(MemberSet* p_MemberSet);
   ~ConstDef(void) noexcept(false);
  
    static bool bIsConstDef(Stz* szConstNm, EnameSet* pTerms);
    static Stz* szConstNm(EnameSet* pTerms);

    Footer* aFooter(void);
  };

  ConstDef* _pConstDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
