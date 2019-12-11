/*============================================================================*/
/* Datum.hpp -- Datum (column value outside of row)                           */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Datum : public Object
  {
  private:
    Column* m_pCol;   // which column
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

    bool bExtractDtm(char** aazCrs, Format* pFmt);
  
    bool bExtractBcd(char** aazCrs, Format* pFmt);
    bool bExtractBol(char** aazCrs, Format* pFmt);
    bool bExtractDte(char** aazCrs, Format* pFmt);
    bool bExtractFlt(char** aazCrs, Format* pFmt);
    bool bExtractInt(char** aazCrs, Format* pFmt);
    bool bExtractIvl(char** aazCrs, Format* pFmt);
    bool bExtractTme(char** aazCrs, Format* pFmt);
    bool bExtractTsp(char** aazCrs, Format* pFmt);
    bool bExtractVch(char** aazCrs, Format* pFmt);

    bool bIsNull(void);
    void vSetNull(void);
  
    // - - - - - - - - - - - - - - - -
  
    AnyVal* aAnyVal(void);
    char*   azChar(void);
    Column* pColumn(void);
    void    vColumn(Column* pColumn);
    void    vCopy(AnyVal* aAnyVal);
    AnyVal* aEmbeddedAnyVal(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    i8      nStackUsage(void);
    Stz*    szStz(void);
    void    vStz(Stz* szStz);
    void    vStz(char* acnChar, i8 nLength);
    void    vStzDump(void);
    i8      nStzSize(void);
    void    vStzSize(i8 nSize);
    i8      nStzSpace(void);
    void    vToString(Stz* szStz);
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
