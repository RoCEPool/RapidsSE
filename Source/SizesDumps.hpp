/*============================================================================*/
/* Dump.hpp -- Class Dumps                                                    */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class Dump
  {
  public:
    static void vDummy(eCN eClass, Object* pObject, const char* aTab);
    static void vEnums(eCN eClass, Object* pObject, const char* aTab);
    static void vInvalid(eCN eClass, Object* pObject, const char* aTab);
    static void vUndefined(eCN eClass, Object* pObject, const char* aTab);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
