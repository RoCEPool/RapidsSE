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
        
  private:
  /*----------------------------------------------------------------------------*/
  /* convert logical cell to cell subscript                                     */
  /*----------------------------------------------------------------------------*/
  inline i8 i_ColToCell(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    
#ifdef __NATURAL_CELL_ORDER
    return i_Col;
#else
    return -1 * i_Col;
#endif
  }
  
  public:

  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline bool bIsNull(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);
      
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
  inline d8 dVal(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);
        
    d8* aD8 = (d8*) & m_maCells[i_Cell];
    
    d8 dVal = * aD8;
    
    return dVal;
  }

  /*----------------------------------------------------------------------------*/
  /* set d8 float                                                               */
  /*----------------------------------------------------------------------------*/
  inline void vVal(i8 i_Col, d8 dValue)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);
    
    d8* aD8 = (d8*) & m_maCells[i_Cell];
    
    * aD8 = dValue;
  }
    
  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline i8 iVal(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);
    
    i8* aI8 = (i8*) & m_maCells[i_Cell];
    
    i8 iVal = * aI8;

    return iVal;
  }

  /*----------------------------------------------------------------------------*/
  /* set i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline void vVal(i8 i_Col, i8 iVal)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);
    
    i8* aI8 = (i8*) & m_maCells[i_Cell];
    
    * aI8 = iVal;
  }

  /*----------------------------------------------------------------------------*/
  /* get Stz* string                                                            */
  /*----------------------------------------------------------------------------*/
  inline Stz* szVal(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    i8 i_Cell = i_ColToCell(i_Col);

    i8 i_Ofs = (i8) m_maCells[i_Cell];
      
    Stz* szStz = aAdd<Stz*>(&m_maCells[i_Cell],i_Ofs);
        
    return szStz;
  }
        
  };
  
  Rsb* _aRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
