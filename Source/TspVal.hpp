/*============================================================================*/
/* TspVal.hpp -- Tsp Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TspVal : public _Header
  {
  private:
  union {
    Tsp   m_wTsp;
    i8    m_iInt;
  };

  public:
    void* operator new(size_t);
    
  public:
    Tsp   wTsp(void);
    void  vTsp(Tsp wTsp);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline TspVal* _aTspVal(void* aAddr)
  {
    TspVal* aTspVal = reinterpret_cast<TspVal*>(aAddr);
  
    RASSERT_TYPE(aTspVal,CN_INTVAL);
  
    return aTspVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
