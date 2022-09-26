/*============================================================================*/
/* TypeMap1D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/

#include "BASE.h"
#include "TypeMap1D.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

// Ref: TM_ALL_NUMERIC
static TypeId axAllNumeric[] = {
    TypeId::xIdOf(CT_INTEGER),
    TypeId::xIdOf(CT_DECIMAL),
    TypeId::xIdOf(CT_FLOAT)
};

// Ref: TM_ALL_STRING
static TypeId axAllString[] = {
    TypeId::xIdOf(CT_CHAR),
    TypeId::xIdOf(CT_VARCHAR)
};

// Ref: TM_ALL_YM_INTERVALS
static TypeId axAllYmIntervals[] = {
    TypeId::xIdOf(IV_YEAR),
    TypeId::xIdOf(IV_MONTH),
    TypeId::xIdOf(IV_YEAR_TO_MONTH),
};

// Ref: TM_ALL_DT_INTERVALS
static TypeId axAllDtIntervals[] = {
    TypeId::xIdOf(IV_DAY),
    TypeId::xIdOf(IV_HOUR),
    TypeId::xIdOf(IV_MINUTE),
    TypeId::xIdOf(IV_SECOND),
    TypeId::xIdOf(IV_DAY_TO_HOUR),
    TypeId::xIdOf(IV_DAY_TO_MINUTE),
    TypeId::xIdOf(IV_DAY_TO_SECOND),
    TypeId::xIdOf(IV_HOUR_TO_MINUTE),
    TypeId::xIdOf(IV_HOUR_TO_SECOND),
    TypeId::xIdOf(IV_MINUTE_TO_SECOND),
};

// Ref: TM_ALL_NON_UNKNOWN
static TypeId axAllNonNull[] = {
    TypeId::xIdOf(CT_BOOLEAN),
    TypeId::xIdOf(CT_CHAR),
    TypeId::xIdOf(CT_DATE),
    TypeId::xIdOf(CT_DECIMAL),
    TypeId::xIdOf(CT_FLOAT),
    TypeId::xIdOf(CT_INTEGER),
    TypeId::xIdOf(IV_YEAR),
    TypeId::xIdOf(IV_MONTH),
    TypeId::xIdOf(IV_YEAR_TO_MONTH),
    TypeId::xIdOf(IV_DAY),
    TypeId::xIdOf(IV_HOUR),
    TypeId::xIdOf(IV_MINUTE),
    TypeId::xIdOf(IV_SECOND),
    TypeId::xIdOf(IV_DAY_TO_HOUR),
    TypeId::xIdOf(IV_DAY_TO_MINUTE),
    TypeId::xIdOf(IV_DAY_TO_SECOND),
    TypeId::xIdOf(IV_HOUR_TO_MINUTE),
    TypeId::xIdOf(IV_HOUR_TO_SECOND),
    TypeId::xIdOf(IV_MINUTE_TO_SECOND),
    TypeId::xIdOf(CT_TIME),
    TypeId::xIdOf(CT_TIMESTAMP),
    TypeId::xIdOf(CT_VARCHAR),
};

// Ref: TM_ALL_INC_UNKNOWN
static TypeId axAllIncNull[] = {
    TypeId::xIdOf(CT_BOOLEAN),
    TypeId::xIdOf(CT_CHAR),
    TypeId::xIdOf(CT_DATE),
    TypeId::xIdOf(CT_DECIMAL),
    TypeId::xIdOf(CT_FLOAT),
    TypeId::xIdOf(CT_INTEGER),
    TypeId::xIdOf(IV_YEAR),
    TypeId::xIdOf(IV_MONTH),
    TypeId::xIdOf(IV_YEAR_TO_MONTH),
    TypeId::xIdOf(IV_DAY),
    TypeId::xIdOf(IV_HOUR),
    TypeId::xIdOf(IV_MINUTE),
    TypeId::xIdOf(IV_SECOND),
    TypeId::xIdOf(IV_DAY_TO_HOUR),
    TypeId::xIdOf(IV_DAY_TO_MINUTE),
    TypeId::xIdOf(IV_DAY_TO_SECOND),
    TypeId::xIdOf(IV_HOUR_TO_MINUTE),
    TypeId::xIdOf(IV_HOUR_TO_SECOND),
    TypeId::xIdOf(IV_MINUTE_TO_SECOND),
    TypeId::xIdOf(CT_TIME),
    TypeId::xIdOf(CT_TIMESTAMP),
    TypeId::xIdOf(CT_VARCHAR),
    TypeId::xIdOf(CT_UNKNOWN),
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TypeMap1D)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeMap1D::TypeMap1D()
 : Object(CN_TYPEMAP1D)
{
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);

  m_iSize = TypeSystem::iMaxTypeId() + 1;
  ASSERT(m_iSize > 0 && m_iSize < 128);

  i8 nBytes = m_iSize * (i8) sizeof(*m_aiTypeArray);
  m_aiTypeArray = (i1*) malloc(nBytes);
  memset(m_aiTypeArray, (int) TypeSystem::iInvalidTypeId(), nBytes);

  _checkInitialised_(CN_TYPEMAP1D);
  _postcondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypeMap1D::~TypeMap1D()
{
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);
  free(m_aiTypeArray);
}

/*----------------------------------------------------------------------------*/
/* Add a mapping of input types to an output type.                            */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap1D* TypeMap1D::pAddMapping(TypeId xArg1, TypeId xResult)
{
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);
  ASSERT(xResult.iIdVal() >= 0 && xResult.iIdVal() < m_iSize);

  m_aiTypeArray[xArg1.iIdVal()] = (i1) xResult.iIdVal();

  return this;
}

/*----------------------------------------------------------------------------*/
/* Add a mapping of input types to an output type.                            */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap1D* TypeMap1D::pAddMapping(eTM eMapper, TypeId xResult)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(eMapper >= TM_NONE && eMapper < TM_ENUMS);
  ASSERT(xResult.iIdVal() >= 0 && xResult.iIdVal() < m_iSize);

  switch (eMapper) {
    case TM_ALL_NUMERICS:
      for (TypeId xVal : axAllNumeric) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_STRINGS:
      for (TypeId xVal : axAllString) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_YM_INTERVALS:
      for (TypeId xVal : axAllYmIntervals) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_DT_INTERVALS:
      for (TypeId xVal : axAllDtIntervals) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_NON_UNKNOWN:
      for (TypeId xVal : axAllNonNull) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_INC_UNKNOWN:
      for (TypeId xVal : axAllIncNull) {
        m_aiTypeArray[xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_SAME:
      NEVER_REACH

    default:
      NEVER_REACH
  }

  return this;
}

/*----------------------------------------------------------------------------*/
/* Add a mapping of one input type to the same type as the output type.       */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap1D* TypeMap1D::pAddSelfMapping(TypeId xArg)
{
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);

  ASSERT(xArg.iIdVal() >= 0 && xArg.iIdVal() < m_iSize);

  m_aiTypeArray[xArg.iIdVal()] = (i1) xArg.iIdVal();

  return this;
}

/*----------------------------------------------------------------------------*/
/* Lookup a mapping of input types to an output type                          */
/*----------------------------------------------------------------------------*/
TypeId TypeMap1D::xLookup(TypeId xArg1)
{
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);

  i1 iResultId = m_aiTypeArray[xArg1.iIdVal()];
  return TypeId{iResultId};
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
