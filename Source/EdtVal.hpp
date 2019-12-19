/*============================================================================*/
/* EdtVal.hpp -- Edt Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class EdtVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Edt   m_wEdt;

  public:
    void* operator new(size_t);
    
  public:
    Edt   wEdt(void);
    void  vEdt(Edt wEdt);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline EdtVal* _aEdtVal(void* aAddr)
  {
    EdtVal* aEdtVal = reinterpret_cast<EdtVal*>(aAddr);
  
    RASSERT_TYPE(aEdtVal,CN_EDTVAL);
  
    return aEdtVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
