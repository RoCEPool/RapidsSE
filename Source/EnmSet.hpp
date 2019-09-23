/*============================================================================*/
/* EnmSet.hpp -- enname string set                                            */
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
      i8  m_i_String;
      eTN m_eToken;
    } m_xEnnPair[C_ENM_SET];

    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    EnmSet(void);
  
   ~EnmSet() noexcept(false);
  
    void vAdd(Stz* szName, Stz* szString, eTN eToken);
    i8   nEnm(void);
    Stz* szString(i8 i_Enm);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
