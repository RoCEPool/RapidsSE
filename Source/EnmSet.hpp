/*============================================================================*/
/* EnmSet.hpp -- ename-value set                                              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  static const i8 C_ENM_SET = 64;
  
  class EnmSet : public Object
  {
  public:
  
    i8 m_nEnm;
  
    struct {
      i8  m_i_Name;
      i8  m_i_Value;
      eTN m_eToken;
    } m_xEnnPair[C_ENM_SET];

    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    EnmSet(void);
   ~EnmSet() noexcept(false);
  
    void vAdd(Stz* szName, Stz* szValue, eTN eToken);
    i8   nEnm(void);
    Stz* szName(i8 i_Enm);
    eTN  eToken(i8 i_Enm);
    Stz* szValue(i8 i_Enm);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
