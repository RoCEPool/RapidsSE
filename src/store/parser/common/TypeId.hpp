/*============================================================================*/
/* TypeId.hpp - A class that represents an indirect reference to a type       */
/*============================================================================*/
#pragma once

#include "eCT.h"
#include "eIV.h"


// Note: This class is currently only used by the SQL compiler but it will
//       eventually be used by the rest of the system. When that occurs this
//       class can be moved to the most appropriate location.
//
// Note: This is a simple class to encapsulate an integer that is used as a
//       key to look up a type. Encapsulating it in this way allows us to search
//       for usage of this class and prevent expected automatic conversions from
//       other types (e.g eCT). It is expected that this class will be passed by
//       value, hence it is not a proper object.

namespace Parser
{
  class TypeId
  {
    private:
    i8 m_iId;

    public:
    explicit TypeId(i8 iId);    // no implicit conversions please

    b1 operator==(const TypeId& rhs);
    b1 operator!=(const TypeId& rhs);

    b1     bIsValid();
    i8     iIdVal();
    TypeId xParentTypeId();
    eCT    eCTVal();
    eIV    eIVVal();
    b1     bIsAbstract();
    Stz*   szNamespace();
    Stz*   szTypeName();

    static TypeId xIdOf(eCT eVal);
    static TypeId xIdOf(eIV eVal);
    static TypeId xIdOf(Stz* szNamespace, Stz* szTypeName);


  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
