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
  
    struct {
      _Header m_wxCellHeader;
      Dtm     m_wxCell;
    } AnyVal_DtmCell;
      
    char m_caStringArea[C_MAX_VCH];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CellSingle(void);
    CellSingle(eSI eStringSemantics);
   ~CellSingle(void) noexcept(false);
  
    // auto conversion
    operator AnyVal*(void);

    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Stz* szStz);
  
    Dtm* aCellAddress(void);
 
    char* azChar(void);
    Stz*  szStz(void);

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
