/*============================================================================*/
/* EnameSet.hpp -- ename-value set                                            */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  static const i8 C_ENAME_SET = 64;
  
  class EnameSet : public Object
  {
  public:
  
    i8 m_nEnames;
  
    struct {
      i8  m_i_Name;
      i8  m_i_Value;
      eTN m_eToken;
    } m_xEname[C_ENAME_SET];

    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    EnameSet(void);
   ~EnameSet() noexcept(false);
  
    void vAdd(Stz* szName, i8 iValue,    eTN eToken);
    void vAdd(Stz* szName, Stz* szValue);
    void vContentDump(void);
    void vCopy(EnameSet* pSrcEnameSet);
    i8   nEnames(void);
    i8   i_Find(Stz* szName);
    bool bHasName(Stz* szName);
    Stz* szName(i8 i_Ename);
    void vPrint(void);
    void vPrintDetailed(void);
    void vReset(void);
    void vSprint(Stz* sArgs);
    eTN  eToken(i8 i_Ename);
    i8   iValue(i8 i_Ename);
    i8   iValue(Stz* szName);
    Stz* szValue(i8 i_Ename);
    Stz* szValue(Stz* szName);

    Footer* aFooter(void);
  };
  
  EnameSet* _pEnameSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
