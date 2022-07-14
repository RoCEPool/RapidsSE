/*============================================================================*/
/* LocalStrMedium.hpp -- Local String Medium                                  */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace BASE
{
  static const i8 C_LOCAL_AREA_MEDIUM = (C_SIZE_MEDIUM_OBJECT - 1024)
                                          - (2 * sizeof(i8));

//class Local;
  class TypeValueDscr;

  class LocalStrMedium : public Object
  {
  public:
    TypeValueDscr* m_aLocal;
    i8             m__dummy0;
  
    char           m_caArea[C_LOCAL_AREA_MEDIUM];

  public:
    LocalStrMedium(void);
   ~LocalStrMedium(void) noexcept(false);
  
    // auto conversion
    operator LocalStrMedium*(void);

    char* azArea(void);

    TypeValueDscr* aLocal(void);
    void  vLocal(TypeValueDscr* aLocal);

  private:
  
  };

  LocalStrMedium* _pLocalStrMedium(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
