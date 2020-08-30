/*============================================================================*/
/* Property.hpp - Property                                                    */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Property : public Object
  {
  private:

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
