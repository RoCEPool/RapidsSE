/*============================================================================*/
/* TypeSystem.hpp - Maintains a record of supported SQL types                 */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"
#include "eCT.h"
#include "eIV.h"

// Forward declarations for use in the 'friend' designation
namespace Parser
{
  class TypeMap1D;
  class TypeMap2D;
  class ExtractMap;
}

namespace STORE
{
  class TypeSystem
  {
    friend class TypeId;
    friend class Parser::TypeMap1D;
    friend class Parser::TypeMap2D;
    friend class Parser::ExtractMap;

    public:
    static i8 iMaxTypeId();

  private:
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

    static i1 iInvalidTypeId();
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
