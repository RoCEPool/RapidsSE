/*============================================================================*/
/* Feature.hpp - Self-Describing Feature                                      */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Feature {
    i8      m_iInt;
    Fact*   m_pFact;
    Object* m_pObject;
    void*   m_aVoid;
  };

  class Property : public Object
  {
  private:

    eEG m_eEg;
    i8  m_iEn;

    Feature m_xFeature;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Property(void);
   ~Property(void) noexcept(false);
  
    eEG eEg(void);
    void vEg(eEG eEg);
  
    i8  iEn(void);
    void vEn(i8 iEn);
  
    Feature& rxFeature(void);

  private:
    Footer* aFooter(void);
  };

  Property* _pProperty(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
