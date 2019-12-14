/*============================================================================*/
/* AnyVal.hpp -- Base Class for a Self-Describing Values                      */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class AnyVal : public _Header
  {
  //protected:
  public:
    union {
      Cell*   m_aFip;
      Object* m_pObj;
      
      i8      m_iBcd;
      i8      m_iBol;
      i8      m_iDte;
      i8      m_iEdt;
      d8      m_dFlt;
      i8      m_iInt;
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
    };
  
  public:
    void* operator new(size_t);
  
    i8      iAsInt(void);
    char*   azChar(void);
    void    vCopy(AnyVal* aSrcVal, i8 nBytes=0);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
    void    vInspect(void);
    i8      iInt(void);
    void    vInt(i8 iInt);
    bool    bIsInt(void);
    Object* pObj(void);
    void    vObj(Object* pObj);
    i8      nStackUsage(void);
    Stz*    szStz(void);
    void    vStz(Stz* szStz);
    i8      nStzSize(void);
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
