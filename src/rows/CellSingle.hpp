/*============================================================================*/
/* CellSingle.hpp -- Segment of Cells                                        */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace Rows
{
  class CellSingle : public Object
  {
  protected:
  
    eSI m_eStringSemantics;  // string
  
    i8  m_i_CellDeposit;
    Dtm m_wxaCellArray[C_MAX_COLUMNS];

    char*  m_azStringAdd;
    char m_caStringArea[C_MAX_RECORD];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CellSingle(eSI eStringSemantics);
   ~CellSingle(void) noexcept(false);
  
    Dtm* aColumCell(i8 i_Col);

    void vDepositColVal(d8 dColVal);
    void vDepositColVal(i8 iColVal);
    void vDepositColVal(Stz* szColVal);
  
    d8   dGetColVal(i8 i_Col);
    i8   iGetColVal(i8 i_Col);
    Stz* szGetColVal(i8 i_Col);
  
    void vPrintStrings(void);
  
    void vPutColVal(i8 i_Col, d8 dColVal);
    void vPutColVal(i8 i_Col, i8 iColVal);
    void vPutColVal(i8 i_Col, Stz* szColVal);
  
    void vReset(void);
  
  private:
  
    Stz* szStringAdd_begin(void);
    void vStringAdd_end(i8 nStringSize);

    Footer* aFooter(void);
  };

  CellSingle* _pCellSingle(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
