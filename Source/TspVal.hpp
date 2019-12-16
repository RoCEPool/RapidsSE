/*============================================================================*/
/* TspVal.hpp -- Tsp Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TspVal : public _Header
  {
  friend class Datums::Datum;
  
  private:
  union {
    Tsp   m_wTsp;
    i8    m_iTsp;
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
  
    RASSERT_TYPE(aTspVal,CN_TSPVAL);
  
    return aTspVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
