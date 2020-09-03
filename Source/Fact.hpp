/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    Fact*   m_pFact;
    i8      m_iInt;
    Object* m_pObject;
    void*   m_aOpaque;
    Text*   m_pText;
  };


  class Fact : public Object
  {
  private:
  
    i8    m_iFactNo;
    Fact* m_pParent;
    s256  m_sNote;

    Property m_xProperty;
  
    FactVec* m_pAttributes; // ... which is a set of sub-facts
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pParent, Stz* szNote=nullptr);
   ~Fact(void) noexcept(false);
  
    void vAddAttribute(Fact* pAttribute);
  
    i8   iFactNo(void);
    void vFactNo(i8 iFactNo);
    
    Stz* szNote(void);
    void vNote(Stz* szNote);
  
    Fact* pParent(void);
    void vParent(Fact* pParent);
  
    Property& rxProperty(void);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
