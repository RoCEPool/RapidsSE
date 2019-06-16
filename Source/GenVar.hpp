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
        i8    m_nSize;      // size of str
        char  m_caChr[256]; // str contents
      } m_xStr;             // value contents as xStr
      i8      m_iInt;       // value contents as iInt
      Object* m_pObj;       // value contents as pPtr
      Value*  m_aVal;       // value contents as pPtr
    } ;
  };
  
  static const i8 C_WSP_GENVAR = C_64K;
  
  class GenVar : public Object
  {
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
  
    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStr);
    
    Value*  aNestedValue(void);
  
    i8      iNestedAsInt(void);
    IntVal* aNestedIntVal(void);
    PtrVal* aNestedPtrVal(void);
    StrVal* aNestedStrVal(void);
  
  private:
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
