/*============================================================================*/
/* Rsb.hpp -- Row Cells in Row Slab                                           */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Rsb
  {
  private:
  
    m8 m_maCells[]; // C++ flexible member array must be last

  public:
    Rsb(void);
   ~Rsb(void) noexcept(false);
      
    i8   i_Cell_lglToNtv(i8 iLogicalCell);
  
    bool bIsNull_ntv(i8 i_Cell);
    d8   dVal_ntv(i8 i_Cell);
    i8   iVal_ntv(i8 i_Cell);
    Stz* szVal_ntv(i8 i_Cell);
  };
  
  Rsb* _aRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
