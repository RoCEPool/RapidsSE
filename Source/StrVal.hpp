/*============================================================================*/
/* StrVal.hpp -- Layout of a Stz string when on stack                         */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVal : public AnyVal
  {
  private:
    char m_caStz[1];
  
  public:
    void* operator new(size_t);
  
  public:
    char* azChar(void);
    i8    nStackUsage(void);
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    i8    nStzSize(void);
    void  vToStz(Stz* szStz);
  };

  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline StrVal* _aStrVal(void* aAddr)
  {
    StrVal* aStrVal = reinterpret_cast<StrVal*>(aAddr);
  
    RASSERT_TYPE(aStrVal,CN_STZVAL);

    return aStrVal;
  }  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
