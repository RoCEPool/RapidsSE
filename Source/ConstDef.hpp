/*============================================================================*/
/* ConstDef.hpp - Constant Definition                                         */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class ConstDef : public Object,
                   public ListItem_t<ConstDef>
  {
  public:
    ConstSet* m_p_ConstSet; // the _p_ sequence means not owned by
  
    // const name
    s32 m_sConstNm;   // name of the constant
    i8  m_iConst;     // value of the constant
  
    TagSet m_oTerms; // terms of this constant

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ConstDef(ConstSet* p_ConstSet);
   ~ConstDef(void) noexcept(false);
  
    static bool bIsConstDef(Stz* szConstNm, TagSet* pTerms);
    static Stz* szConstNm(TagSet* pTerms);

    Stz* szConstNm(void);
    void vConstNm(Stz* szConstNm);

    i8 iConst(void);

    void vPrint(i8 i_Tab);

    void vTerms(TagSet& roTerms);

    Footer* aFooter(void);
  };

  ConstDef* _pConstDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
