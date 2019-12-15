/*============================================================================*/
/* AnyVal.hpp -- Base Class for a Self-Describing Values                      */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class AnyVal : public _Header
  {
  protected:
  public:
    union {
      Cell*   m_aFip;
      d8      m_dFlt;
      i8      m_iInt;
      Object* m_pObj;

      i8      m_iBcd;
      i8      m_iBol;
      i8      m_iDte;
      i8      m_iEdt;
      i8      m_iIvl;
      i8      m_iTme;
      i8      m_iTsc;
      i8      m_iTsp;

      _Dte    m_xDte;
      _Edt    m_xEdt;
      _Ivl    m_xIvl;
      _Tme    m_xTme;
      _Tsp    m_xTsp;
      
      Bcd     m_wBcd;
      Bol     m_wBol;
      Dte     m_wDte;
      Edt     m_wEdt;
      Flt     m_wFlt;
      Ivl     m_wIvl;
      Tme     m_wTme;
      Tsp     m_wTsp;

      i8      m_nBytesFollowing;
    } u;
  
  public:
    void* operator new(size_t);
  
    i8      iAsInt(void);
    Bcd     wBcd(void);
    void    vBcd(Bcd wBcd);
    Bol     wBol(void);
    void    vBol(Bol wBol);
    char*   azChar(void);
    void    vCopy(AnyVal* aSrcVal, i8 nBytes=0);
    Dte     wDte(void);
    void    vDte(Dte wDte);
    Edt     wEdt(void);
    void    vEdt(Edt wEdt);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
