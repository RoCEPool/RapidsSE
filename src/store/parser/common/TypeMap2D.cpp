/*============================================================================*/
/* TypeMap2D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/

#include "BASE.h"
#include "TypeMap2D.hpp"
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
__aFooterFunction(TypeMap2D)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeMap2D::TypeMap2D()
 : Object(CN_TYPEMAP2D)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  m_iSize = TypeSystem::iMaxTypeId() + 1;
  ASSERT(m_iSize > 0 && m_iSize < 128);

  i8 nBytes = m_iSize * m_iSize * (i8) sizeof(*m_aiTypeArray);
  m_aiTypeArray = (i1*) malloc(nBytes);
  memset(m_aiTypeArray, (int) TypeSystem::iInvalidTypeId(), nBytes);

  _checkInitialised_(CN_TYPEMAP2D);
  _postcondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypeMap2D::~TypeMap2D()
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);
  free(m_aiTypeArray);
}

/*----------------------------------------------------------------------------*/
/* Add a mapping of input types to an output type.                            */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap2D* TypeMap2D::pAddMapping(TypeId xArg1, TypeId xArg2, TypeId xResult)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);
  ASSERT(xArg2.iIdVal() >= 0 && xArg2.iIdVal() < m_iSize);
  ASSERT(xResult.iIdVal() >= 0 && xResult.iIdVal() < m_iSize);

  m_aiTypeArray[xArg1.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();

  return this;
}


/*----------------------------------------------------------------------------*/
/* Add a mapping of input types to an output type.                            */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap2D* TypeMap2D::pAddMapping(TypeId xArg1, eTM eMapper, TypeId xResult)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);
  ASSERT(eMapper >= TM_NONE && eMapper < TM_ENUMS);
  ASSERT(xResult.iIdVal() >= 0 && xResult.iIdVal() < m_iSize);

  switch (eMapper) {
    case TM_ALL_NUMERICS:
      for (TypeId xVal : axAllNumeric) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_STRINGS:
      for (TypeId xVal : axAllString) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;
      
    case TM_ALL_YM_INTERVALS:
      for (TypeId xVal : axAllYmIntervals) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;
      
    case TM_ALL_DT_INTERVALS:
      for (TypeId xVal : axAllDtIntervals) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;
      
    case TM_ALL_NON_UNKNOWN:
      for (TypeId xVal : axAllNonNull) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;
      
    case TM_ALL_INC_UNKNOWN:
      for (TypeId xVal : axAllIncNull) {
        m_aiTypeArray[xArg1.iIdVal() * m_iSize + xVal.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;
      
    case TM_SAME:
      m_aiTypeArray[xArg1.iIdVal() * m_iSize + xArg1.iIdVal()] = (i1) xResult.iIdVal();
      break;
      
    default:
      NEVER_REACH
  }
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* Add a mapping of input types to an output type.                            */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap2D* TypeMap2D::pAddMapping(eTM eMapper, TypeId xArg2, TypeId xResult)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(eMapper >= TM_NONE && eMapper < TM_ENUMS);
  ASSERT(xArg2.iIdVal() >= 0 && xArg2.iIdVal() < m_iSize);
  ASSERT(xResult.iIdVal() >= 0 && xResult.iIdVal() < m_iSize);

  switch (eMapper) {
    case TM_ALL_NUMERICS:
      for (TypeId xVal : axAllNumeric) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_STRINGS:
      for (TypeId xVal : axAllString) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_YM_INTERVALS:
      for (TypeId xVal : axAllYmIntervals) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_DT_INTERVALS:
      for (TypeId xVal : axAllDtIntervals) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_NON_UNKNOWN:
      for (TypeId xVal : axAllNonNull) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_ALL_INC_UNKNOWN:
      for (TypeId xVal : axAllIncNull) {
        m_aiTypeArray[xVal.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      }
      break;

    case TM_SAME:
      m_aiTypeArray[xArg2.iIdVal() * m_iSize + xArg2.iIdVal()] = (i1) xResult.iIdVal();
      break;

    default:
      NEVER_REACH
  }

  return this;
}

/*----------------------------------------------------------------------------*/
/* Add a mapping where the input types are the same as the output type.       */
/* Returns a ^ptr for chaining consecutive calls.                             */
/*----------------------------------------------------------------------------*/
TypeMap2D* TypeMap2D::pAddSelfMapping(TypeId xArg)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(xArg.iIdVal() >= 0 && xArg.iIdVal() < m_iSize);

  m_aiTypeArray[xArg.iIdVal() * m_iSize + xArg.iIdVal()] = (i1) xArg.iIdVal();

  return this;
}

/*----------------------------------------------------------------------------*/
/* Lookup a mapping of input types to an output type                          */
/*----------------------------------------------------------------------------*/
TypeId TypeMap2D::xLookup(TypeId xArg1, TypeId xArg2)
{
  _precondition_(_MAGIC_,CN_TYPEMAP2D,_FOOTER_);

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);
  ASSERT(xArg2.iIdVal() >= 0 && xArg2.iIdVal() < m_iSize);

  i1 iResultId = m_aiTypeArray[xArg1.iIdVal() * m_iSize + xArg2.iIdVal()];
  return TypeId{iResultId};
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
