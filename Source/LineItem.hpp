/*============================================================================*/
/* LineItem.hpp -- TPC-H Line Item                                            */
/*============================================================================*/
#pragma once

namespace MockEE
{
  /*
   * This is only the leading portion of line item.
   */
  
  class LineItem : public Object
  {
  public:
    Int  m_iOrderKey; //
    Int  m_iPartKey;  //
    Int  m_iSuppKey;  //
    Int  m_iLineNo;   //
    Bcd  m_iQuantity; //
    Bcd  m_iExtPrice; //
    Bcd  m_iDiscount; //
    Bcd  m_iTax;      //
  
   public:
  
  };
  
  LineItem* _pLineItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
