/*============================================================================*/
/* StrVal.hpp -- Layout of a String when on Stack                             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVal : public AnyVal
  {
  protected:
    char m_caStr[1];
  
  public:
    void* operator new(size_t);
  
  public:
    void  vAssign(Stz* szStr);
    char* azChar(void);
    i8    nSize(void);
    Stz*  szStz(void);

    i8    nStackUsage(void);
    void  vToString(Stz* szStz);
  };

  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline StrVal* _aStrVal(void* aAddr)
  {
    StrVal* aStrVal = reinterpret_cast<StrVal*>(aAddr);
  
    RASSERT_TYPE(aStrVal,CN_STRVAL);

    return aStrVal;
  }  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
