/*============================================================================*/
/* EnumFlags.hpp -- Class Flags                                              */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class EnumFlags : public Object
  {
  private:
    bool m_bAll;
    bool m_bEnum[256];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pAddr);
  
    EnumFlags(void);
    ~EnumFlags() noexcept(false);
  
    bool bAll(void);
    void vAll(bool bAll);
  
    bool bEnum(i8 i_Enum);
    void vEnum(i8 i_Enum, bool bFlag);
  
    bool bFlagged(i8 i_Enum);
  
    Footer* aFooter(void);
  };
  
  EnumFlags* _pEnumFlags(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
