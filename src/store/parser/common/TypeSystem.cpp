/*============================================================================*/
/* TypeSystem.hpp - Maintains a record of supported SQL types                 */
/*============================================================================*/

#include "BASE.h"
#include "TypeSystem.hpp"
#include "eIV.h"

using namespace Parser;

// A description of a type in the system
typedef struct {
  i8   m_iTypeId;
  i8   m_iParentId;
  eCT  m_eCTVal;
  eIV  m_eIVVal;         // interval sub-type value
  b1   m_bIsAbstract;
  Stz* m_szNamespace;
  Stz* m_szTypeName;
} TypeDescriptor;

static Stz* C_DEFAULT_NAMESPACE = "rapids"_Stz;

// The set of known types in the system, indexed by their typeId.
// When we fully support dynamic types then this will change to a
// proper indexed tree.
static i8 C_MAX_TYPE_ID = 0;
static TypeDescriptor axTypes[] = {
 {C_MAX_TYPE_ID++, -1, CT_BOOLEAN,     IV_NONE,             false, aCT[CT_BOOLEAN],          C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_CHAR,        IV_NONE,             false, aCT[CT_CHAR],             C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_DATE,        IV_NONE,             false, aCT[CT_DATE],             C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_DECIMAL,     IV_NONE,             false, aCT[CT_DECIMAL],          C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_FLOAT,       IV_NONE,             false, aCT[CT_FLOAT],            C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_INTEGER,     IV_NONE,             false, aCT[CT_INTEGER],          C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_TIME,        IV_NONE,             false, aCT[CT_TIME],             C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_TIMESTAMP,   IV_NONE,             false, aCT[CT_TIMESTAMP],        C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_VARCHAR,     IV_NONE,             false, aCT[CT_VARCHAR],          C_DEFAULT_NAMESPACE },
 {C_MAX_TYPE_ID++, -1, CT_UNKNOWN,     IV_NONE,             false, aCT[CT_UNKNOWN],          C_DEFAULT_NAMESPACE },

{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_YM, IV_YEAR,             false, aIV[IV_YEAR],             C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_YM, IV_MONTH,            false, aIV[IV_MONTH],            C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_YM, IV_YEAR_TO_MONTH,    false, aIV[IV_YEAR_TO_MONTH],    C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY,              false, aIV[IV_DAY],              C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR,             false, aIV[IV_HOUR],             C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_MINUTE,           false, aIV[IV_MINUTE],           C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_SECOND,           false, aIV[IV_SECOND],           C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_HOUR,      false, aIV[IV_DAY_TO_HOUR],      C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_MINUTE,    false, aIV[IV_DAY_TO_MINUTE],    C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_DAY_TO_SECOND,    false, aIV[IV_DAY_TO_SECOND],    C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR_TO_MINUTE,   false, aIV[IV_HOUR_TO_MINUTE],   C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_HOUR_TO_SECOND,   false, aIV[IV_HOUR_TO_SECOND],   C_DEFAULT_NAMESPACE },
{ C_MAX_TYPE_ID++, -1, CT_INTERVAL_DT, IV_MINUTE_TO_SECOND, false, aIV[IV_MINUTE_TO_SECOND], C_DEFAULT_NAMESPACE },
};


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeSystem::TypeSystem()
{
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypeSystem::~TypeSystem()
{
 
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Get the typeId of the parent of this type                                  */
/*----------------------------------------------------------------------------*/
i8 TypeSystem::iParentId(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_iParentId;
}

/*----------------------------------------------------------------------------*/
/* Get the eCT value of the parent of this type. May be CT_NONE.              */
/*----------------------------------------------------------------------------*/
eCT TypeSystem::eCTVal(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_eCTVal;
}

/*----------------------------------------------------------------------------*/
/* Get the eIV value of the parent of this type. May be IV_NONE.              */
/*----------------------------------------------------------------------------*/
eIV TypeSystem::eIVVal(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_eIVVal;
}

/*----------------------------------------------------------------------------*/
/* Return true if this type is an abstract type and cannot be instantiated.   */
/*----------------------------------------------------------------------------*/
b1 TypeSystem::bIsAbstract(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_bIsAbstract;
}

/*----------------------------------------------------------------------------*/
/* Get the namespace to which this type belongs.                              */
/*----------------------------------------------------------------------------*/
Stz* TypeSystem::szNamespace(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_szNamespace;
}

/*----------------------------------------------------------------------------*/
/* Get the name of this type (unique within its namespace).                   */
/*----------------------------------------------------------------------------*/
Stz* TypeSystem::szTypeName(i8 iTypeId)
{
  ASSERT(iTypeId >= 0);
  ASSERT(iTypeId < C_MAX_TYPE_ID);

  return axTypes[iTypeId].m_szTypeName;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
