/*============================================================================*/
/* Numbat.hpp - number analyser                                               */
/*============================================================================*/
#pragma once

namespace Resources
{
  struct NumSet
  {
    Peruse m_oPeruse[8][8];
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Numbat : public Object
  {
  public:
    s32 m_sName;
  
    NumSet m_oNumSet[8];
  
    // must be last
    Footer m_xFooter;
    
  public:
    Numbat(Stz* szName);
    ~Numbat() noexcept(false);
  
    void vAdd(i8 iArg);
    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
