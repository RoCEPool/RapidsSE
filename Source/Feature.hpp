/*============================================================================*/
/* Feature.hpp - Self-Describing Feature                                      */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    Fact*   m_pFact;
    i8      m_iInt;
    Object* m_pObject;
    void*   m_aOpaque;
  };

  class Feature : public Object
  {
  private:

    eEG m_eEg;
    i8  m_iEn;

    ePR m_eProperty;
    Property m_xProperty;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Feature(void);
   ~Feature(void) noexcept(false);
  
    eEG eEg(void);
    void vEg(eEG eEg);
  
    i8  iEn(void);
    void vEn(i8 iEn);
  
    ePR eProperty(void);
    void vProperty(ePR eProperty);
    Property& rxProperty(void);

  private:
    Footer* aFooter(void);
  };

  Feature* _pFeature(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
