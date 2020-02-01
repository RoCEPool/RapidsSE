/*============================================================================*/
/* PtrVal.hpp -- Layout of a Pointer to a Proper Object when on Stack         */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class PtrVal : public _Header
  {
  template <i8 SPACE> friend class Datums::Datum_t;
  
  private:
  union {
    Object* m_pObj;
    i8      m_iObj;
  };

  public:
    void* operator new(size_t);
    
  public:
    Object* pObj(void);
    void    vObj(Object* pObject);
    i8      nStackUsage(void);
    void    vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline PtrVal* _aPtrVal(void* aAddr)
  {
    PtrVal* aPtrVal = reinterpret_cast<PtrVal*>(aAddr);
  
    RASSERT_TYPE(aPtrVal,CN_PTRVAL);
  
    return aPtrVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
