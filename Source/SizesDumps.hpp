/*============================================================================*/
/* Dump.hpp -- Class Dumps                                                    */
/*============================================================================*/
#pragma once

namespace Oblique
{
  class Dump
  {
  public:
    static void vDummy(eCN eClass, Object* pObject, Stz* aTab);
    static void vEnums(eCN eClass, Object* pObject, Stz* aTab);
    static void vInvalid(eCN eClass, Object* pObject, Stz* aTab);
    static void vUndefined(eCN eClass, Object* pObject, Stz* aTab);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
