/*============================================================================*/
/* AnyVal.hpp -- Base Class for a Self-Describing Values                      */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class AnyVal : public _Header
  {
  template <i8 SPACE> friend class Datums::Datum_t;

  protected:
    union {
      i8      m__zero__;
      
      ForthCell*   m_aFip;
      d8      m_dFlt;
      i8      m_iInt;
      Object* m_pObj;
      
      Bcd     m_wBcd;
      Bol     m_wBol;
      Dte     m_wDte;
      Edt     m_wEdt;
      Flt     m_wFlt;
      Int     m_wInt;
      Ivl     m_wIvl;
      Tme     m_wTme;
      Tsp     m_wTsp;

      i8      m_nBytesFollowing;
    };
  
  public:
    void* operator new(size_t);
  
    i8      iAsInt(void);
    Bcd     wBcd(void);
    void    vBcd(Bcd wBcd);
    Bol     wBol(void);
    void    vBol(Bol wBol);
    char*   azChar(void);
    Dtm*    aCell(void);
    void    vCopy(AnyVal* aSrcVal, i8 nBytes=0);
    Dte     wDte(void);
    void    vDte(Dte wDte);
    Edt     wEdt(void);
    void    vEdt(Edt wEdt);
    ForthCell*   aFip(void);
    void    vFip(ForthCell* aFip);
    Flt     wFlt(void);
    void    vFlt(Flt wFlt);
    void    vInspect(void);
    i8      iInt(void);
    void    vInt(i8 iInt);
    Ivl     wIvl(void);
    void    vIvl(Ivl wIvl);
    bool    bIsInt(void);
    Object* pObj(void);
    void    vObj(Object* pObj);
    i8      nStackUsage(void);
    Stz*    szStz(void);
    void    vStz(Stz* szStz);
    i8      nStzSize(void);
    Tme     wTme(void);
    void    vTme(Tme wTme);
    Tsp     wTsp(void);
    void    vTsp(Tsp wTsp);
    void    vToString(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline AnyVal* _aAnyVal(void* aAddr)
  {
    AnyVal* aAnyVal = reinterpret_cast<AnyVal*>(aAddr);
  
    return aAnyVal;
  }
}

////////////////////////////////////////////////////////////////////////////////

// concrete variations

class AnyVal_y32 : public AnyVal
{
  byte m_yaSpace[32];
  
public:
  void vInit(void) {
    MemLib::vZero(&m__zero__,sizeof(AnyVal_y32) - 8,CN_NONE);
    vMagic();
    vInt(C_DATUM_NULL);
  }
};


class AnyVal_y8KB : public AnyVal
{
  byte m_yaSpace[C_8KB];
  
public:
  void vInit(void) {
    MemLib::vZero(&m__zero__,sizeof(AnyVal_y8KB) - 8,CN_NONE);
    vMagic();
    vInt(C_DATUM_NULL);
  }
};

class AnyVal_y48KB : public AnyVal
{
  byte m_yaSpace[C_48KB];
  
public:
  void vInit(void) {
    MemLib::vZero(&m__zero__,sizeof(AnyVal_y48KB) - 8,CN_NONE);
    vMagic();
    vInt(C_DATUM_NULL);
  }
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
