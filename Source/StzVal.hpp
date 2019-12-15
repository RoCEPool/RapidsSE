/*============================================================================*/
/* StzVal.hpp -- memory layout of an Stz value                                */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class StzVal : public _Header
  {
  friend class Datums::Datum;
  
  private:
    i8   m_nBytesFollowing;
    char m_caStz[1];
  
  public:
    void* operator new(size_t);
  
  public:
    char* azChar(void);
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
