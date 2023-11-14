/*============================================================================*/
/* TypeSystem.hpp - Maintains a record of supported SQL types                 */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"
#include "eCT.h"
#include "eIV.h"


// Note: This class is currently only used by the SQL compiler but it will
//       eventually be used by the rest of the system. When that occurs this
//       class can be moved to the most appropriate location.
namespace Parser
{
  class TypeSystem
  {

// CRAIG TO LOOK AT : Alka commented because TypeSystem was moved from Parser to Store namespace

//    friend class TypeId;
//    friend class TypeMap1D;
//    friend class TypeMap2D;
//    friend class ExtractMap;

    public:
    static i8 iMaxTypeId();

  // CRAIG TO LOOK AT : Alka commented because TypeSystem was moved from Parser to Store namespace
  //private:
    TypeSystem(); // Not to be initialised. Static class only

    static b1 bIsValid(TypeId xTypeId);

    static TypeId xParentTypeId(TypeId xTypeId);
    static eCT    eCTVal(TypeId xTypeId);
    static eIV    eIVVal(TypeId xTypeId);
    static b1     bIsAbstract(TypeId xTypeId);
    static Stz*   szNamespace(TypeId xTypeId);
    static Stz*   szTypeName(TypeId xTypeId);

    static TypeId xIdOf(eCT eVal);
    static TypeId xIdOf(eIV eVal);
    static TypeId xIdOf(Stz* szNamespace, Stz* szTypeName);

    static i8 iInvalidTypeId();
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
