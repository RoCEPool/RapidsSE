/*============================================================================*/
/* Finalizers.hpp -- Class Finalizers                                         */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Finalize
  {
  public:
    static void vEnums(eCN eClass, Object* pObject);
    static void vUndefined(eCN eClass, Object* pObject);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
