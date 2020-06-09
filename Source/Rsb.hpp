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
      
    i8   i_ColToCell_ntv(i8 i_Col);
  
    bool bIsNull_ntv(i8 i_Cell_ntv);
    d8   dVal_ntv(i8 i_Cell_ntv);
    i8   iVal_ntv(i8 i_Cell_ntv);
    Stz* szVal_ntv(i8 i_Cell_ntv);
  };
  
  Rsb* _aRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
