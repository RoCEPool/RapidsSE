/*============================================================================*/
/* EnmSet.hpp -- enname string set                                            */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  static const i8 C_ENM_PAIRS = 64;
  
  class EnmSet : public Object
  {
  public:
  
    i8 m_nEno;
  
    struct {
      i8  m_i_Name;
      i8  m_i_String;
      eTN m_eToken;
    } m_xEnnPair[C_ENM_PAIRS];

    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    EnmSet(void);
  
   ~EnmSet() noexcept(false);
  
    void vAdd(Stz* szName, Stz* szString, eTN eToken);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
