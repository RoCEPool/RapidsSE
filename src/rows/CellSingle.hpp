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
        
    TypedCell m_xTypedCell;
  

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CellSingle(void);
   ~CellSingle(void) noexcept(false);
  
    // auto conversion
    operator TypedCell*(void);

    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStz);
  
    eCN  eAnyClass(void);
  
    Dtm* aCellAddress(void);
 
    char* azChar(void);
  
   ForthCell* aFip(void);

    Stz*  szStz(void);

    d8   dGetColVal(void);
    i8   iGetColVal(void);
    Stz* szGetColVal(void);
  
    bool bIsIntVal(void);
  
    void vPrintString(void);
  
    void vPutColVal(d8 dColVal);
    void vPutColVal(i8 iColVal);
    void vPutColVal(Object* pObject);

    void vToString(Stz* szDst);

  
  private:
  
    Footer* aFooter(void);
  };

  CellSingle* _pCellSingle(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
