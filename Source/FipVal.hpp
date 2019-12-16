/*============================================================================*/
/* FipVal.hpp -- Layout of a Forth Instruction Pointer when on Stack          */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class FipVal : public _Header
  {
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
  union {
    Cell*  m_aFip;
    i8     m_iFip;
  };

  public:
    void* operator new(size_t);
    
  public:
    Cell* aFip(void);
    void  vFip(Cell* aFip);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline FipVal* _aFipVal(void* aAddr)
  {
    FipVal* aFipVal = reinterpret_cast<FipVal*>(aAddr);
  
    RASSERT_TYPE(aFipVal,CN_FIPVAL);
  
    return aFipVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
