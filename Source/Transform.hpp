/*============================================================================*/
/* Transform.hpp - Transform                                                  */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Transform : public Object
  {
  private:
 
    Kbase* m_pKbase; //

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Transform(Kbase* pKbase);
   ~Transform(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Transform* _pTransform(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
