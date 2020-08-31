/*============================================================================*/
/* Feature.hpp - Self-Describing Feature                                      */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    i8      m_iInt;
    Fact*   m_pFact;
    Object* m_pObject;
    void*   m_aVoid;
  };

  class Feature : public Object
  {
  private:

    eEG m_eEg;
    i8  m_iEn;

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
  
    Property& rxProperty(void);

  private:
    Footer* aFooter(void);
  };

  Feature* _pFeature(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
