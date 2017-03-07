/*============================================================================*/
/* Dump.hpp -- Class Dumps                                                    */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Dump
  {
  public:
    static void vDummy(eCN eClass, Object* pObject, Str* aTab);
    static void vEnums(eCN eClass, Object* pObject, Str* aTab);
    static void vInvalid(eCN eClass, Object* pObject, Str* aTab);
    static void vUndefined(eCN eClass, Object* pObject, Str* aTab);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
