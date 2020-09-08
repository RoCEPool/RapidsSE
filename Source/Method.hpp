/*============================================================================*/
/* Method.hpp - Kbase Methods                                                  */
/*============================================================================*/
#pragma once

namespace Facts
{
  static const i8 C_QUESTION_CODE = C_2KB;

  //----------------------------------------------------------------------------

  class Method : public Object
  {
  private:
  
    i8  m_i_Code;
    eQC m_eaCode[C_QUESTION_CODE];    // { QC_code [<bytes>] }

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Method(void);
   ~Method(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Method* _pMethod(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
