/*============================================================================*/
/* ValWsp.hpp - workspace for manipulating values                             */
/*============================================================================*/
#pragma once

//#define OLD_VALWSP

#ifdef OLD_VALWSP
namespace Values
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
  
  class ValWsp : public Value
  {
  private:
    union {
      WspFxd<C_64K> m_xWsp;         // workspace
      ValSee        m_xNestedValue; // inspector
    };
    Footer          m_xFooter;
  
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Str* aStr);
    void vAssignBinary(Object* pObject);

    char*  aWspArea(void);
    Value* pNestedValue(void);
    Value* pNestedValueSelf(void);

    i8     iNestedAsInt(void);
  
    Footer* pFooter(void);
  };
  
  ValWsp* _pValWsp(void* aAddr);
}
#else
namespace Values
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
  
  class ValWsp : public Object
  {
  private:
    union {
      WspFxd<C_64K> m_xWsp;         // workspace
      ValSee        m_xNestedValue; // inspector
    };
    Footer          m_xFooter;
    
    public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Str* aStr);
    void vAssignBinary(Object* pObject);
    
    char*  aWspArea(void);
    Value* pNestedValue(void);
    Value* pNestedValueSelf(void);
  
    i8     iNestedAsInt(void);
    IntVal* pNestedIntVal(void);
    PtrVal* pNestedPtrVal(void);
    StrVal* pNestedStrVal(void);
  
    Footer* pFooter(void);
  };
  
  ValWsp* _pValWsp(void* aAddr);
}
#endif
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
