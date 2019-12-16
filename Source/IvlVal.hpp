/*============================================================================*/
/* IntVal.hpp -- Ivl Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class IvlVal : public _Header
  {
  friend class Datums::Datum_abstract;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
  union {
    Ivl   m_wIvl;
    i8    m_iIvl;
  };

  public:
    void* operator new(size_t);
    
  public:
    Ivl   wIvl(void);
    void  vIvl(Ivl wIvl);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline IvlVal* _aIvlVal(void* aAddr)
  {
    IvlVal* aIvlVal = reinterpret_cast<IvlVal*>(aAddr);
  
    RASSERT_TYPE(aIvlVal,CN_IVLVAL);
  
    return aIvlVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
