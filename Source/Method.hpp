/*============================================================================*/
/* Method.hpp - Kbase Methods                                                  */
/*============================================================================*/
#pragma once

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
  
  private:
    void vGenMethod000(void);

    Footer* aFooter(void);
  };

  Method* _pMethod(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
