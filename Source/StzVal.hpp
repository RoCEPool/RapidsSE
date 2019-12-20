/*============================================================================*/
/* StzVal.hpp -- memory layout of an Stz value                                */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class StzVal : public _Header
  {
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    i8   m_nBytesFollowing;
    char m_caStz[1];
  
  public:
    void* operator new(size_t);
  
  public:
    char* azChar(void);
  
    void vSetMax(void)                {  }  // set max value
    void vSetMid(void)                {  }  // set mid value
    void vSetMin(void)                { }  // set min value
    void vSetNull(void)              {  }  // set to be null
    void vSetUsual(void)            {  }  // set usual value

    i8    nStackUsage(void);
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    void  vStz(char* anChar, i8 nLength);
    i8    nStzSize(void);
    void  vStzSize(i8 nSize);
    void  vToStz(Stz* szStz);
  };

  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline StzVal* _aStzVal(void* aAddr)
  {
    StzVal* aStzVal = reinterpret_cast<StzVal*>(aAddr);
  
    RASSERT_TYPE(aStzVal,CN_STZVAL);

    return aStzVal;
  }  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
