/*============================================================================*/
/* TypeSystem.hpp - Maintains a record of supported SQL types                 */
/*============================================================================*/
#pragma once

#include "eCT.h"
#include "eIV.h"


// Note: This class is currently only used by the SQL compiler but it will
//       eventually be used by the rest of the system. When that occurs this
//       class can be moved to the most appropriate location.
namespace Parser
{
  class TypeSystem
  {
    private:
    TypeSystem(); // Not to be initialised. Static class only
    ~TypeSystem();


    public:
    static i8   iParentId(i8 iTypeId);
    static eCT  eCTVal(i8 iTypeId);
    static eIV  eIVVal(i8 iTypeId);
    static b1   bIsAbstract(i8 iTypeId);
    static Stz* szNamespace(i8 iTypeId);
    static Stz* szTypeName(i8 iTypeId);

    static constexpr i8 iIdOf(eCT eVal);
    static constexpr i8 iIdOf(eIV eVal);
    static i8 iIdOf(Stz* szNamespace, Stz* szTypeName);

  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
