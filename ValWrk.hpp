/*============================================================================*/
/* ValWrk.hpp - workspace for manipulating values                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  // the nested value inspector
  class ValSee
  {
    Value    m_xValue;      // the Value header
    union {
      struct {
        i8   m_nSize;       // size of str
        char m_cChr[256];   // str contents
      } m_xStr;             // value contents as xStr
      i8     m_iInt;        // value contents as iInt
      Value* m_pPtr;        // value contents as pPtr
    };
  };
  
  class ValWrk : public Value
  {
  private:
    union {
      WspFxd<C_64K> m_xWsp;         // workspace
      ValSee        m_xNestedValue; // inspector
    };
    Footer          m_xFooter;
    
    public:
    void* operator new(size_t);
    ValWrk(void);
    ~ValWrk(void) noexcept(false);
  
    void operator =(Bcd xBcd);
    void operator =(Bol xBol);
    void operator =(Dte xDte);
    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Ivl xIvl);
    void operator =(Object* pObject);
    void operator =(Value* aValue);
    void operator =(Stz* aStr);
    void operator =(Tme xTme);
    void operator =(Tsp xTsp);
    void vAssignBinary(Object* pObject);
    
    char*  aWspArea(void);
    Value* pNestedValue(void);
    Value* pNestedValueSelf(void);
  
    BinVal* pNestedBinVal(void);
    i8      iNestedAsInt(void);
    IntVal* pNestedIntVal(void);
    PtrVal* pNestedPtrVal(void);
    StrVal* pNestedStrVal(void);
    void    vToString(Stz* aWorkspace);
    
    Footer* aFooter(void);
  };
  
  ValWrk* _pValWrk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
