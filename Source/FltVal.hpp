/*============================================================================*/
/* FltVal.hpp -- Flt Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class FltVal : public _Header
  {
  friend class Datums::_Datum_;
  
  private:
  union {
    Flt   m_wFlt;
    i8    m_iFlt;
  };

  public:
    void* operator new(size_t);
    
  public:
    Flt   wFlt(void);
    void  vFlt(Flt wFlt);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline FltVal* _aFltVal(void* aAddr)
  {
    FltVal* aFltVal = reinterpret_cast<FltVal*>(aAddr);
  
    RASSERT_TYPE(aFltVal,CN_FLTVAL);
  
    return aFltVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
