/*============================================================================*/
/* TypedCell.hpp -- Self-Describing / Dynamically Typed Cell                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class TypedCell
  {
  public:
  
    Dtm  m_wxTypeCell;
    Dtm  m_wxValueCell;

  public:
    TypedCell(void);
   ~TypedCell(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
