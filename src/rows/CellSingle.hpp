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
  
    union {
      
      struct {
        _Header m_wxCellHeader;
        Dtm     m_wxCell;
      } DtmCell;
      
      AnyVal  m_xAnyVal;
    
      FipVal  m_xFipVal;
      PtrVal  m_xPtrVal;
    
      BcdVal  m_xBcdVal;
      BolVal  m_xBolVal;
      DteVal  m_xDteVal;
      EdtVal  m_xEdtVal;
      FltVal  m_xFltVal;
      IntVal  m_xIntVal;
      IvlVal  m_xIvlVal;
      TmeVal  m_xTmeVal;
      TspVal  m_xTspVal;
    };

    eSI m_eStringSemantics;  // string

    char m_caStringArea[C_MAX_VCH];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CellSingle(eSI eStringSemantics);
   ~CellSingle(void) noexcept(false);
  
    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Stz* szStz);
  
    Dtm* aCellAddress(void);
 
    d8   dGetColVal(void);
    i8   iGetColVal(void);
    Stz* szGetColVal(void);
  
    void vPrintString(void);
  
    void vPutColVal(d8 dColVal);
    void vPutColVal(i8 iColVal);
    void vPutColVal(Stz* szColVal);
  
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
