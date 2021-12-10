/*============================================================================*/
/* Permit.hpp - Permit                                                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Permit : public Object
  {
  public:
    union
    {
      struct
      {
        u8 m_uBits    : 16; //
        u8 m_uAllowed : 48; //
      };
      u8 m_uAsU8;
    };
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Permit(void);
   ~Permit(void) noexcept(false);
      
    bool bMayContinue(void);
    void vSetBit(i8 i_Bit);
    bool bTestBit(i8 i_Bit);
  
    Footer* aFooter(void);
  };

  Permit* _pPermit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
