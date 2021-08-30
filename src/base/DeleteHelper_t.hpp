/*============================================================================*/
/* DeleteHelper_t.hpp -- a delete helper                                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DeleteHelper_t
  {
  public:
    Object* m_pDeleter;
    Stz*    m_szFunc;
  
  public:
    DeleteHelper_t(void);
   ~DeleteHelper_t(void) noexcept(false);
  
    void vDeleteBy(Object* pDeleter, Stz* szFunc);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
