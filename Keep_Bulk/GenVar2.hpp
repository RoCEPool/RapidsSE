/*============================================================================*/
/* GenVar2.hpp - Generic Variable  (which contains a nested value)            */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class GenVar2 : public Object
  {
  private:
    union {
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
  
    GenVar2(void);
   ~GenVar2(void) noexcept(false);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    char*   azChar(void);
    eCN     eEmbeddedClass(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    i8      nSize(void);
    SenVar* aEmbeddedSenVar(void);
    Stz*    szStz(void);
  
  private:
    Footer* aFooter(void);
  };

  GenVar2* _pGenVar2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
