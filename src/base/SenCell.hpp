/*============================================================================*/
/* SenCell.hpp -- SEN Cell                                                    */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace BASE
{
  class SenCell : public Object
  {
  protected:
    TypeValueArea m_xTypeValueArea;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SenCell(void);
   ~SenCell(void) noexcept(false);
  
    // auto conversion
    operator SenCell*(void);
    operator TypeValueArea*(void);

    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStz);
  
   
    char* azChar(void);
  
    CodeCell* aFip(void);

    d8   dGetColVal(void);
    i8   iGetColVal(void);
    Stz* szGetColVal(void);
  
    bool bIsIntVal(void);
  
    void vPrintString(void);
  
    void vPutColVal(d8 dColVal);
    void vPutColVal(i8 iColVal);
    void vPutColVal(Object* pObject);

    i8   nStackUsage(void);
    Stz* szStz(void);
    i8   nStzSize(void);

    void vToString(String* psStz);
    eCN  eType(void);
    TypeValue* aTypeValue(void);
    TypeValueArea* aTypeValueArea(void);

  private:
  
    Footer* aFooter(void);
  };

  SenCell* _pSenCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
