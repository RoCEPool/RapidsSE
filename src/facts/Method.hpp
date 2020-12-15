/*============================================================================*/
/* Method.hpp - Kbase Methods                                                  */
/*============================================================================*/
#pragma once

#include "eTQ.h"

namespace Facts
{
  static const i8 C_METHOD_CODE  = C_4KB;
  static const i8 C_METHOD_STACK = C_4KB;

  //----------------------------------------------------------------------------

  class Method : public Object
  {
  private:
  
    Kbase* m_pKbase; //
    Fact*  m_pSelf;  //
    Fact*  m_pSubA;  //
    Fact*  m_pSubB;  //

    i8  m_i_Code;
    eTQ m_eaCode[C_METHOD_CODE]; // { TQ_code [<bytes>] }
  
    i8 m_iaDataStack[C_METHOD_STACK];


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Method(Kbase* pKbase, Fact* pSelf);
   ~Method(void) noexcept(false);
  
    void vAddCode(eTQ eCode);
    void vAddCodeAdr(eTQ eCode, Opaque* aOpaque);
    void vAddCodeInt(eTQ eCode, i8 iArg);
    void vAddCodeStz(eTQ eCode, Stz* szStz);
  
    void vExecute(Fact* pAnchor);
    
    eTQ  eGetCode(i8 i_Code);
  
    void vGenMethod(i8 iMethodNo);
  
    void vSchedule(Fact** ppAnchor);
  
    void vTransformAdd(Stz* szTransformNm);
    void vTransformDelete(Fact** ppTransform);
  
    void vTQ_ACCESS_METHODS(Fact** ppTransform);
    void vTQ_ADD_ASM_HANDLERS(Fact** ppTransform);
    void vTQ_ADD_ASM_SUBROUTINES(Fact** ppTransform);
    void vTQ_ADD_OPERATOR(Fact** ppTransform);
    void vTQ_ADD_OUTPUT_STAGE(Fact** ppTransform);
    void vTQ_ADD_PIPELINE(Fact** ppTransform);
    void vTQ_ADD_START_STAGE(Fact** ppTransform);
    void vTQ_FROM_RESOLUTION(Fact** ppTransform);
    void vTQ_IMPORT_TABLES(Fact** ppTransform);
    void vTQ_INITIAL_PLAN(Fact** ppTransform);
    void vTQ_OUTPUT_STREAMDEF(Fact** ppTransform);
    void vTQ_PROJECT_RESOLUTION(Fact** ppTransform);
    void vTQ_WRITE_EOASM(Fact** ppTransform);
    void vTQ_WRITE_PIPELINE(Fact** ppTransform);

  private:
    void vGenMethod000(void);

    Footer* aFooter(void);
  };

  Method* _pMethod(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
