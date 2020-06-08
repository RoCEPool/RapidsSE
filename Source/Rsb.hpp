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
      
    i8   i_Cell(i8 iLogicalCell);
  
    bool bIsNull(i8 i_Cell);
    d8   dVal(i8 i_Cell);
    i8   iVal(i8 i_Cell);
    Stz* szVal(i8 i_Cell);
  };
  
  Rsb* _aRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
