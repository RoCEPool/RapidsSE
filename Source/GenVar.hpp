/*============================================================================*/
/* GenVar.hpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class GenVar : public Object
  {
  private:

    AllVal u;

    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar(void);
    GenVar(AnyVal*);
   ~GenVar(void) noexcept(false);
  
    // auto conversion
    operator AnyVal*(void);

    void operator =(AnyVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    char*   azChar(void);
    void    vCopy(AnyVal* aAnyVal);
    AnyVal* aEmbeddedAnyVal(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    i8      nStackUsage(void);
    Stz*    szStz(void);
    i8      nStzSize(void);
    void    vToString(Stz* szStz);
    eCN     eValClass(void);

  private:
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
