/*============================================================================*/
/* Property.hpp - Self-Describing Property                                    */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Property : public Object
  {
  private:

    eEG m_eEg;
    i2  m_iEn;

    union {
      i8      m_iInt;
      Fact*   m_pFact;
      Object* m_pObject;
      void*   m_aVoid;
    };
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Property(void);
   ~Property(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Property* _pProperty(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
