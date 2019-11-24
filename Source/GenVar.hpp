/*============================================================================*/
/* GenVar.hpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class _NestedValue
  {
  public:
    _Header  m_xHeader;
    i8       m_wWord;
    char     m_caStr[C_MAX_VCH2];

  };
  
  //----------------------------------------------------------------------------
  
  class GenVar : public Object
  {
  private:

    union {
      _NestedValue  m_xNstVal;
      
      SenVar  m_xSenVar;
      
      FipVar  m_xFipVar;
      IntVar  m_xIntVar;
      PtrVar  m_xPtrVar;
      StrVar  m_xStrVar;
    } u;

    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar(void);
   ~GenVar(void) noexcept(false);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    char*   azChar(void);
    eCN     eEmbeddedClass(void);
    StkVal* aEmbeddedStkVal(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    i8      nSize(void);
    Stz*    szStz(void);
  
  private:
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
