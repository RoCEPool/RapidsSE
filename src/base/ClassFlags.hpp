/*============================================================================*/
/* ClassFlags.hpp -- Enum Flags                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ClassFlags : public Object
  {
  private:
    bool m_bAll;
    bool m_bEnum[CN_ENUMS];

    // must be last
    Footer m_xFooter;
    
  public:  
    ClassFlags(void);
   ~ClassFlags() noexcept(false);
  
    bool bAll(void);
    void vAll(bool bAll);
  
    bool bEnum(i8 i_Enum);
    void vEnum(i8 i_Enum, bool bFlag);
  
    bool bFlagged(i8 i_Enum);
  
    Footer* aFooter(void);
  };
  
  ClassFlags* _pClassFlags(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
