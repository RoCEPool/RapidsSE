/*============================================================================*/
/* DeleteHelper.hpp -- a delete helper                                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DeleteHelper
  {
  public:
    Object* m_pDeleter;
    Stz*    m_szFunc;
  
  public:
    DeleteHelper(void);
   ~DeleteHelper(void) noexcept(false);
  
    void vDeleteBy(Object* pDeleter, Stz* szFunc);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
