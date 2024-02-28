/*============================================================================*/
/* TypeSystem.hpp - Maintains a record of supported SQL types                 */
/*============================================================================*/

#include "BASE.h"
#include "TypeSystem.hpp"
#include "eIV.h"

using namespace STORE;

// A description of a type in the system
typedef struct {
  i8   m_iTypeId;
  i8   m_iParentId;
  eCT  m_eCTVal;
  eIV  m_eIVVal;         // interval sub-type value
  b1   m_bIsAbstract;
  Stz* m_szNamespace;
  Stz** m_ppszTypeName;  // Note: Stz**, not Stz*. See explanation below.
} TypeDescriptor;

// NOTE ON Stz**:
// The member m_ppszTypeName is a Stz** and not a Stz* for a good reason.
// Because the axTypes[] array below is a static variable, it is initialised
// before main() is entered. However this array references another global array
// aCT[], and the order that these two arrays are initialised is not strict.
// Hence if axTypes[] is initialised first then the value of aCT[CT_BOOLEAN] (for example)
// will initially be a nullptr. So instead we store the address of aCT[CT_BOOLEAN]
// and at runtime when we need to look this up then aCT[CT_BOOLEAN] will have
// been initialised by then.

static Stz* C_DEFAULT_NAMESPACE = "rapids"_Stz;
static constexpr i8 C_INVALID_TYPEID = -1;

// The set of known types in the system, indexed by their typeId.
// When we fully support dynamic types then this will change to a
// proper indexed tree.
static i8 C_NEXT_TYPE_ID = 0;
static TypeDescriptor axTypes[] = {
/*00*/  { C_NEXT_TYPE_ID++, -1, CT_BOOLEAN,     IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_BOOLEAN])          },
/*01*/  { C_NEXT_TYPE_ID++, -1, CT_CHAR,        IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_CHAR])             },
/*02*/  { C_NEXT_TYPE_ID++, -1, CT_DATE,        IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_DATE])             },
/*03*/  { C_NEXT_TYPE_ID++, -1, CT_DECIMAL,     IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_DECIMAL])          },
/*04*/  { C_NEXT_TYPE_ID++, -1, CT_FLOAT,       IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_FLOAT])            },
/*05*/  { C_NEXT_TYPE_ID++, -1, CT_INTEGER,     IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_INTEGER])          },
/*06*/  { C_NEXT_TYPE_ID++, -1, CT_TIME,        IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_TIME])             },
/*07*/  { C_NEXT_TYPE_ID++, -1, CT_TIMESTAMP,   IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_TIMESTAMP])        },
/*08*/  { C_NEXT_TYPE_ID++, -1, CT_VARCHAR,     IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_VARCHAR])          },
/*09*/  { C_NEXT_TYPE_ID++, -1, CT_UNKNOWN,     IV_NONE,             false, C_DEFAULT_NAMESPACE, &(aCT[CT_UNKNOWN])          },

/*10*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_YM, IV_YEAR,             false, C_DEFAULT_NAMESPACE, &(aIV[IV_YEAR])             },
/*11*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_YM, IV_MONTH,            false, C_DEFAULT_NAMESPACE, &(aIV[IV_MONTH])            },
/*12*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_YM, IV_YEAR_TO_MONTH,    false, C_DEFAULT_NAMESPACE, &(aIV[IV_YEAR_TO_MONTH])    },
/*13*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY,              false, C_DEFAULT_NAMESPACE, &(aIV[IV_DAY])              },
/*14*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR,             false, C_DEFAULT_NAMESPACE, &(aIV[IV_HOUR])             },
/*15*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_MINUTE,           false, C_DEFAULT_NAMESPACE, &(aIV[IV_MINUTE])           },
/*16*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_SECOND,           false, C_DEFAULT_NAMESPACE, &(aIV[IV_SECOND])           },
/*17*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_HOUR,      false, C_DEFAULT_NAMESPACE, &(aIV[IV_DAY_TO_HOUR])      },
/*18*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_MINUTE,    false, C_DEFAULT_NAMESPACE, &(aIV[IV_DAY_TO_MINUTE])    },
/*19*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_SECOND,    false, C_DEFAULT_NAMESPACE, &(aIV[IV_DAY_TO_SECOND])    },
/*20*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR_TO_MINUTE,   false, C_DEFAULT_NAMESPACE, &(aIV[IV_HOUR_TO_MINUTE])   },
/*21*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR_TO_SECOND,   false, C_DEFAULT_NAMESPACE, &(aIV[IV_HOUR_TO_SECOND])   },
/*22*/  { C_NEXT_TYPE_ID++, -1, CT_INTERVAL_DT, IV_MINUTE_TO_SECOND, false, C_DEFAULT_NAMESPACE, &(aIV[IV_MINUTE_TO_SECOND]) },
};

constexpr i8 C_MAXTYPEID = (sizeof(axTypes) / sizeof(*axTypes)) - 1; // number of entries in the array


// Reverse mapping of eCT to typeId
static constexpr i8 aiTypeIdOfCT[CT_ENUMS] = {
    /*  0 CT_NONE         */ -1,
    /*  1 CT_BOOLEAN      */ 0,
    /*  2 CT_CHAR         */ 1,
    /*  3 CT_DATE         */ 2,
    /*  4 CT_DECIMAL      */ 3,
    /*  5 CT_FLOAT        */ 4,
    /*  6 CT_INTEGER      */ 5,
    /*  7 CT_INTERVAL_DT  */ -1,
    /*  8 CT_INTERVAL_YM  */ -1,
    /*  9 CT_TIME         */ 6,
    /* 10 CT_TIMESTAMP    */ 7,
    /* 11 CT_VARCHAR      */ 8,
    /* 12 CT_UNKNOWN      */ 9
};

// Reverse mapping of eIV to typeId
static constexpr i8 aiTypeIdOfIV[IV_ENUMS] = {
    /* 0  IV_NONE                         */ -1,
    /* 1  IV_SUBCATEGORY_YEARMONTH_START  */ -1,
    /* 2  IV_YEAR                         */ 10,
    /* 3  IV_MONTH                        */ 11,
    /* 4  IV_YEAR_TO_MONTH                */ 12,
    /* 5  IV_SUBCATEGORY_YEARMONTH_END    */ -1,
    /* 6  IV_SUBCATEGORY_DAYTIME_START    */ -1,
    /* 7  IV_DAY                          */ 13,
    /* 8  IV_HOUR                         */ 14,
    /* 9  IV_MINUTE                       */ 15,
    /* 10 IV_SECOND                       */ 16,
    /* 11 IV_DAY_TO_HOUR                  */ 17,
    /* 12 IV_DAY_TO_MINUTE                */ 18,
    /* 13 IV_DAY_TO_SECOND                */ 19,
    /* 14 IV_HOUR_TO_MINUTE               */ 20,
    /* 15 IV_HOUR_TO_SECOND               */ 21,
    /* 16 IV_MINUTE_TO_SECOND             */ 22,
    /* 17 IV_SUBCATEGORY_DAYTIME_END      */ -1,
};


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeSystem::TypeSystem()
{
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Check that this is a valid type ID                                         */
/*----------------------------------------------------------------------------*/
b1 TypeSystem::bIsValid(TypeId xTypeId)
{
  return (xTypeId.iIdVal() > C_INVALID_TYPEID) && (xTypeId.iIdVal() <= C_MAXTYPEID);
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of the parent of this type                                  */
/*----------------------------------------------------------------------------*/
TypeId TypeSystem::xParentTypeId(TypeId xTypeId)
{
  ASSERT(bIsValid(xTypeId));

  return TypeId{axTypes[xTypeId.iIdVal()].m_iParentId};
}

/*----------------------------------------------------------------------------*/
/* Get the eCT value of the parent of this type. May be CT_NONE.              */
/*----------------------------------------------------------------------------*/
eCT TypeSystem::eCTVal(TypeId xTypeId)
{
//  ASSERT(bIsValid(xTypeId) || xTypeId.iIdVal() == C_INVALID_TYPEID);

  if (xTypeId.iIdVal() == C_INVALID_TYPEID) {
    return CT_NONE;
  }

  return axTypes[xTypeId.iIdVal()].m_eCTVal;
}

/*----------------------------------------------------------------------------*/
/* Get the eIV value of the parent of this type. May be IV_NONE.              */
/*----------------------------------------------------------------------------*/
eIV TypeSystem::eIVVal(TypeId xTypeId)
{
  ASSERT(bIsValid(xTypeId) || xTypeId.iIdVal() == C_INVALID_TYPEID);

  if (xTypeId.iIdVal() == C_INVALID_TYPEID) {
    return IV_NONE;
  }

  return axTypes[xTypeId.iIdVal()].m_eIVVal;
}

/*----------------------------------------------------------------------------*/
/* Return true if this type is an abstract type and cannot be instantiated.   */
/*----------------------------------------------------------------------------*/
b1 TypeSystem::bIsAbstract(TypeId xTypeId)
{
  ASSERT(bIsValid(xTypeId));

  return axTypes[xTypeId.iIdVal()].m_bIsAbstract;
}

/*----------------------------------------------------------------------------*/
/* Get the namespace to which this type belongs.                              */
/*----------------------------------------------------------------------------*/
Stz* TypeSystem::szNamespace(TypeId xTypeId)
{
  ASSERT(bIsValid(xTypeId));

  return axTypes[xTypeId.iIdVal()].m_szNamespace;
}

/*----------------------------------------------------------------------------*/
/* Get the name of this type (unique within its namespace).                   */
/*----------------------------------------------------------------------------*/
Stz* TypeSystem::szTypeName(TypeId xTypeId)
{
  ASSERT(bIsValid(xTypeId));

  return *(axTypes[xTypeId.iIdVal()].m_ppszTypeName);
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given eCT type.                                        */
/* Should not be used for interval types. Instead use iIdOf(eIV).             */
/*----------------------------------------------------------------------------*/
TypeId TypeSystem::xIdOf(eCT eVal)
{
  ASSERT(eVal >= CT_NONE);
  ASSERT(eVal < CT_ENUMS);
  ASSERT(eVal != CT_INTERVAL_DT);
  ASSERT(eVal != CT_INTERVAL_YM);

  TypeId xTypeId = TypeId{aiTypeIdOfCT[eVal]};

  return xTypeId;
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given eIV type.                                        */
/*----------------------------------------------------------------------------*/
TypeId TypeSystem::xIdOf(eIV eVal)
{
  ASSERT(eVal >= IV_NONE);
  ASSERT(eVal < IV_ENUMS);
  ASSERT( (eVal > IV_SUBCATEGORY_YEARMONTH_START && eVal < IV_SUBCATEGORY_YEARMONTH_END) ||
          (eVal > IV_SUBCATEGORY_DAYTIME_START   && eVal < IV_SUBCATEGORY_DAYTIME_END) );

  TypeId xTypeId = TypeId{aiTypeIdOfIV[eVal]};
  ASSERT(bIsValid(xTypeId));

  return xTypeId;
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given namespace and typename. Currently a slow search. */
/* Returns an invalid TypeId if not found (check bIsValid()).                 */
/*----------------------------------------------------------------------------*/
TypeId TypeSystem::xIdOf(Stz* szNamespace, Stz* szTypeName)
{
  ASSERT(szNamespace);
  ASSERT(szTypeName);

	for (i8 i=0; i <= C_MAXTYPEID; i++) {

		if (axTypes[i].m_szNamespace->bSame(szNamespace) &&
						((*(axTypes[i].m_ppszTypeName))->bSame(szTypeName)  ||
						aIV_field2[axTypes[i].m_eIVVal]->bSame(szTypeName))) {
      return TypeId{axTypes[i].m_iTypeId};
    }

  }

  // Not found
  return TypeId{C_INVALID_TYPEID};
}

/*----------------------------------------------------------------------------*/
/* Returns the maximum integer typeId in the system                           */
/*----------------------------------------------------------------------------*/
i8 TypeSystem::iMaxTypeId()
{
  return C_MAXTYPEID;
}

/*----------------------------------------------------------------------------*/
/* Returns the integer value for invalid type IDs in the system               */
/*----------------------------------------------------------------------------*/
i8 TypeSystem::iInvalidTypeId()
{
  return C_INVALID_TYPEID;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
