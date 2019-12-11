/*============================================================================*/
/* Datum.hpp -- Datum (column value outside of row)                           */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Datum : public Object
  {
  private:
  public:
    AllVal  u;        // column value

  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum(void);
    Datum(AnyVal*);
   ~Datum(void) noexcept(false);
  
    // auto conversion
    operator AnyVal*(void);
  
    void operator =(AnyVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    bool bIsNull(void);
    void vSetNull(void);
  
    // - - - - - - - - - - - - - - - -
  
    AnyVal* aAnyVal(void);
    char*   azChar(void);
    i8      iBcd(void);
    void    vBcd(i8 iBcd);
    i8      iBol(void);
    void    vBol(i8 iBol);
    void    vCopy(AnyVal* aAnyVal);
    i8      iDte_day(void);
    i8      iDte_month(void);
    i8      iDte_year(void);
    AnyVal* aEmbeddedAnyVal(void);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
    d8      dFlt(void);
    void    vFlt(d8 dFlt);
    i8      iInt(void);
    void    vInt(i8 iInt);
    i8      iIvl(void);
    bool    bIsInt(void);
    Object* pObj(void);
    void    vObj(Object* pObj);
    i8      nStackUsage(void);
    Stz*    szStz(void);
    void    vStz(Stz* szStz);
    void    vStz(char* acnChar, i8 nLength);
    void    vStzDump(void);
    i8      nStzSize(void);
    void    vStzSize(i8 nSize);
    i8      nStzSpace(void);
    i8      iTme(void);
    void    vTme(i8 iTme);
    void    vToString(Stz* szStz);
    i8      iTsp(void);
    void    vTsp(i8 iTsp);
    eCN     eValClass(void);

  public:
    Footer* aFooter(void);
  };
  
  Datum* _pDatum(void* aAddr);
}


#pragma pack(push, 1)
class Nibbler1 {
  struct {
    u8 m_uNibble : 4 ; //
  } m_xaNibbles[128];
};
class Nibbler2 {
  struct {
    u8 m_uNibbleL : 4 ; //
    u8 m_uNibbleR : 4 ; //
  } m_xaNibbles[64];
};
#pragma pack(pop)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
