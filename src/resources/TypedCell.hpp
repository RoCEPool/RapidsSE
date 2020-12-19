/*============================================================================*/
/* TypedCell.hpp -- Self-Describing / Dynamically Typed Cell                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class TypedCell
  {
  public:
  
    //Dtm  m_wxTypeCell;
    _Header m_wxCellHeader;
    Dtm     m_wxValueCell;

  public:
    TypedCell(void);
   ~TypedCell(void) noexcept(false);
  
    eCN  eType(void);
    void vType(eCN eType);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
