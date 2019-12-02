/*============================================================================*/
/* StzVal.hpp -- memory layout of an Stz value                                */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StzVal : public AnyVal
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
  inline StzVal* _aStzVal(void* aAddr)
  {
    StzVal* aStrVal = reinterpret_cast<StzVal*>(aAddr);
  
    RASSERT_TYPE(aStrVal,CN_STZVAL);

    return aStrVal;
  }  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
