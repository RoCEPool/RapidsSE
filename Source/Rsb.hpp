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
      
//    i8   i_ColToCell_ntv(i8 i_Col);
//
//    bool bIsNull_ntv(i8 i_Cell_ntv);
//    d8   dVal_ntv(i8 i_Cell_ntv);
//    i8   iVal_ntv(i8 i_Cell_ntv);
//    Stz* szVal_ntv(i8 i_Cell_ntv);
  
  
  /*----------------------------------------------------------------------------*/
  /* convert logical cell to cell subscript                                     */
  /*----------------------------------------------------------------------------*/
  inline i8 i_ColToCell_ntv(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    
    return -1 * i_Col;
  }

  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline bool bIsNull_ntv(i8 i_Cell)
  {
    ASSERT(i_Cell <= 0);
    
    i8* aI8 = (i8*) & m_maCells[i_Cell];
    
    i8 iVal = * aI8;

    if (C_DATUM_NULL == iVal)
      return true;
    else
      return false;
  }

  /*----------------------------------------------------------------------------*/
  /* get d8 float                                                               */
  /*----------------------------------------------------------------------------*/
  inline d8 dVal_ntv(i8 i_Cell)
  {
    ASSERT(i_Cell <= 0);
    
    d8* aD8 = (d8*) & m_maCells[i_Cell];
    
    d8 dVal = * aD8;
    
    return dVal;
  }

  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline i8 iVal_ntv(i8 i_Cell)
  {
    ASSERT(i_Cell <= 0);
    
    i8* aI8 = (i8*) & m_maCells[i_Cell];
    
    i8 iVal = * aI8;

    return iVal;
  }

  /*----------------------------------------------------------------------------*/
  /* get Stz* string                                                            */
  /*----------------------------------------------------------------------------*/
  inline Stz* szVal_ntv(i8 i_Cell)
  {
    ASSERT(i_Cell <= 0);
    
  #ifdef WITH_RSB_STROFS

    i8 i_Ofs = (i8) m_maCells[i_Cell];
      
    Stz* szStz = aAdd<Stz*>(&m_maCells[i_Cell],i_Ofs);
    
  #else
    
    Stz* szStz = (Stz*) m_maCells[i_Cell];

  #endif
    
    return szStz;
  }

  };
  
  Rsb* _aRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
