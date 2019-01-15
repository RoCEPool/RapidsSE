/*============================================================================*/
/* GenVar.hpp - Generic Variable                                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  // the nested value inspector
  class ValSee
  {
    Header   m_xHeader;    // header
    union {
      struct {
        i8    m_nSize;     // size of str
        char  m_cChr[256]; // str contents
      } m_xStr;            // value contents as xStr
      i8      m_iInt;      // value contents as iInt
      Object* m_pObj;      // value contents as pPtr
      Value*  m_aVal;      // value contents as pPtr
    } ;
  };
  
  class GenVar : public Object
  {
    static const i8 C_WSP_GENVAR = C_64K;
  
  private:
    union {
      
      Wsp_t<C_WSP_GENVAR> m_xWsp; // workspace
      
      ValSee m_xNestedValue;      // inspector
    } ;
  
    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar(void);
    ~GenVar(void) noexcept(false);
  
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
    
    char*  aWspArea(void);
    Value* aNestedValue(void);
  
    i8      iNestedAsInt(void);
    IntVal* aNestedIntVal(void);
    PtrVal* aNestedPtrVal(void);
    StrVal* aNestedStrVal(void);
    void    vToString(Stz* aStz);
    
    Footer* aFooter(void);
  };
  
  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
