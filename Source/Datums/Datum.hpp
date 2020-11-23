/*============================================================================*/
/* Datum.hpp -- Datum (column value outside of row)                           */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Datum_h : public Object
  {
  private:
    i8  m_nSizeSpace;
  
    union {
      AnyVal  m_xAnyVal;
    
      FipVal  m_xFipVal;
      PtrVal  m_xPtrVal;
    
      BcdVal  m_xBcdVal;
      BolVal  m_xBolVal;
      DteVal  m_xDteVal;
      EdtVal  m_xEdtVal;
      FltVal  m_xFltVal;
      IntVal  m_xIntVal;
      IvlVal  m_xIvlVal;
      StzVal  m_xStzVal; // this is the larger
      TmeVal  m_xTmeVal;
      TspVal  m_xTspVal;
    };
  
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Datum_h(i8 nSizeSpace);
   ~Datum_h(void) noexcept(false);
  
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
  
    eCN     eAnyClass(void);
    AnyVal* aAnyVal(void);
    char*   azChar(void);
    i8      iBcd(void);
    void    vBcd(i8 iBcd);
    i8      iBol(void);
    void    vBol(i8 iBol);
    void    vCopy(AnyVal* aAnyVal);
    i8      iDte(void);
    i8      iDte_day(void);
    i8      iDte_month(void);
    i8      iDte_year(void);
    void    vDte(i8 iDte);
    void    vDte(i8 iYear, i8 iMonth, i8 iDay);
    i8      iEdt(void);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
    d8      dFlt(void);
    void    vFlt(d8 dFlt);
    i8      iInt(void);
    void    vInt(i8 iInt);
    i8      iIvl(void);
    void    vIvl(i8 iIvl);
    void    vIvl(i8 iSecond, i8 iMicro);
    bool    bIsInt(void);
    Object* pObj(void);
    void    vObj(Object* pObj);
    byte*   ayaSpace(void);
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
    void    vTme(i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro);
    void    vToString(Stz* szStz);
    i8      iTsp(void);
    void    vTsp(i8 iTsp);
    void    vTsp(i8 iYear, i8 iMonth,  i8 iDay,
                 i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro);

  public:
    Footer* aFooter(void);
  };
  

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  template <i8 SPACE>
  class Datum_t : public Datum_h
  {
    byte m_yaSpace[SPACE];
  
    // must be last
    Footer m_xFooter;
  
  public:
    Datum_t(void);
  
    void operator =(AnyVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);
  
  };
}

/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
Datum_t<SPACE>::Datum_t(void)
: Datum_h(SPACE)
{
}

/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
void Datum_t<SPACE>::operator =(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  eCN eClass = aAnyVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL: vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
void Datum_t<SPACE>::operator =(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
void Datum_t<SPACE>::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
void Datum_t<SPACE>::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
template <i8 SPACE>
void Datum_t<SPACE>::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vStz(szStz);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
