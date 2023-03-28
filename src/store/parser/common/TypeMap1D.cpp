/*============================================================================*/
/* TypeMap1D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/

#include "BASE.h"
#include "TypeMap1D.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

static TypeId axAllNumeric[3];      // Ref: TM_ALL_NUMERIC
static TypeId axAllString[2];       // Ref: TM_ALL_STRING
static TypeId axAllYmIntervals[3];  // Ref: TM_ALL_YM_INTERVALS
static TypeId axAllDtIntervals[10]; // Ref: TM_ALL_DT_INTERVALS
static TypeId axAllNonNull[22];     // Ref: TM_ALL_NON_UNKNOWN
static TypeId axAllIncNull[23];     // Ref: TM_ALL_INC_UNKNOWN

/*----------------------------------------------------------------------------*/
/* Initialise static arrays on first use. See note below.                     */
/*----------------------------------------------------------------------------*/
static void vInitStaticArrays()
{
  static bool bInitialised = false;

  if (bInitialised) {
    return;
  }

  // This is really ugly, but necessary because the TypeMap2D gets
  // instantiated and mappings are set before we hit main(). And without
  // this, the TypeMap2D can be used before it's static members are actually
  // initialised. So this forces initialisation to occur on first use.

  i8 i;

  // Ref: TM_ALL_NUMERIC
  i=0;
  /* 0*/ axAllNumeric[i++] = TypeId::xIdOf(CT_INTEGER);
  /* 1*/ axAllNumeric[i++] = TypeId::xIdOf(CT_DECIMAL);
  /* 2*/ axAllNumeric[i++] = TypeId::xIdOf(CT_FLOAT);
  ASSERT(i == sizeof(axAllNumeric)/sizeof(axAllNumeric[0]));

  // Ref: TM_ALL_STRING
  i=0;
  /* 0*/ axAllString[i++] = TypeId::xIdOf(CT_CHAR);
  /* 1*/ axAllString[i++] = TypeId::xIdOf(CT_VARCHAR);
  ASSERT(i == sizeof(axAllString)/sizeof(axAllString[0]));

  // Ref: TM_ALL_YM_INTERVALS
  i=0;
  /* 0*/ axAllYmIntervals[i++] = TypeId::xIdOf(IV_YEAR);
  /* 1*/ axAllYmIntervals[i++] = TypeId::xIdOf(IV_MONTH);
  /* 2*/ axAllYmIntervals[i++] = TypeId::xIdOf(IV_YEAR_TO_MONTH);
  ASSERT(i == sizeof(axAllYmIntervals)/sizeof(axAllYmIntervals[0]));

  // Ref: TM_ALL_DT_INTERVALS
  i=0;
  /* 0*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_DAY);
  /* 1*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_HOUR);
  /* 2*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_MINUTE);
  /* 3*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_SECOND);
  /* 4*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_DAY_TO_HOUR);
  /* 5*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_DAY_TO_MINUTE);
  /* 6*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_DAY_TO_SECOND);
  /* 7*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_HOUR_TO_MINUTE);
  /* 8*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_HOUR_TO_SECOND);
  /* 9*/ axAllDtIntervals[i++] = TypeId::xIdOf(IV_MINUTE_TO_SECOND);
  ASSERT(i == sizeof(axAllDtIntervals)/sizeof(axAllDtIntervals[0]));

  // Ref: TM_ALL_NON_UNKNOWN
  i=0;
  /* 0*/ axAllNonNull[i++] = TypeId::xIdOf(CT_BOOLEAN);
  /* 1*/ axAllNonNull[i++] = TypeId::xIdOf(CT_CHAR);
  /* 2*/ axAllNonNull[i++] = TypeId::xIdOf(CT_DATE);
  /* 3*/ axAllNonNull[i++] = TypeId::xIdOf(CT_DECIMAL);
  /* 4*/ axAllNonNull[i++] = TypeId::xIdOf(CT_FLOAT);
  /* 5*/ axAllNonNull[i++] = TypeId::xIdOf(CT_INTEGER);
  /* 6*/ axAllNonNull[i++] = TypeId::xIdOf(IV_YEAR);
  /* 7*/ axAllNonNull[i++] = TypeId::xIdOf(IV_MONTH);
  /* 8*/ axAllNonNull[i++] = TypeId::xIdOf(IV_YEAR_TO_MONTH);
  /* 9*/ axAllNonNull[i++] = TypeId::xIdOf(IV_DAY);
  /*10*/ axAllNonNull[i++] = TypeId::xIdOf(IV_HOUR);
  /*11*/ axAllNonNull[i++] = TypeId::xIdOf(IV_MINUTE);
  /*12*/ axAllNonNull[i++] = TypeId::xIdOf(IV_SECOND);
  /*13*/ axAllNonNull[i++] = TypeId::xIdOf(IV_DAY_TO_HOUR);
  /*14*/ axAllNonNull[i++] = TypeId::xIdOf(IV_DAY_TO_MINUTE);
  /*15*/ axAllNonNull[i++] = TypeId::xIdOf(IV_DAY_TO_SECOND);
  /*16*/ axAllNonNull[i++] = TypeId::xIdOf(IV_HOUR_TO_MINUTE);
  /*17*/ axAllNonNull[i++] = TypeId::xIdOf(IV_HOUR_TO_SECOND);
  /*18*/ axAllNonNull[i++] = TypeId::xIdOf(IV_MINUTE_TO_SECOND);
  /*19*/ axAllNonNull[i++] = TypeId::xIdOf(CT_TIME);
  /*20*/ axAllNonNull[i++] = TypeId::xIdOf(CT_TIMESTAMP);
  /*21*/ axAllNonNull[i++] = TypeId::xIdOf(CT_VARCHAR);
  ASSERT(i == sizeof(axAllNonNull)/sizeof(axAllNonNull[0]));

  // Ref: TM_ALL_INC_UNKNOWN
  i=0;
  /* 0*/ axAllIncNull[i++] = TypeId::xIdOf(CT_BOOLEAN);
  /* 1*/ axAllIncNull[i++] = TypeId::xIdOf(CT_CHAR);
  /* 2*/ axAllIncNull[i++] = TypeId::xIdOf(CT_DATE);
  /* 3*/ axAllIncNull[i++] = TypeId::xIdOf(CT_DECIMAL);
  /* 4*/ axAllIncNull[i++] = TypeId::xIdOf(CT_FLOAT);
  /* 5*/ axAllIncNull[i++] = TypeId::xIdOf(CT_INTEGER);
  /* 6*/ axAllIncNull[i++] = TypeId::xIdOf(IV_YEAR);
  /* 7*/ axAllIncNull[i++] = TypeId::xIdOf(IV_MONTH);
  /* 8*/ axAllIncNull[i++] = TypeId::xIdOf(IV_YEAR_TO_MONTH);
  /* 9*/ axAllIncNull[i++] = TypeId::xIdOf(IV_DAY);
  /*10*/ axAllIncNull[i++] = TypeId::xIdOf(IV_HOUR);
  /*11*/ axAllIncNull[i++] = TypeId::xIdOf(IV_MINUTE);
  /*12*/ axAllIncNull[i++] = TypeId::xIdOf(IV_SECOND);
  /*13*/ axAllIncNull[i++] = TypeId::xIdOf(IV_DAY_TO_HOUR);
  /*14*/ axAllIncNull[i++] = TypeId::xIdOf(IV_DAY_TO_MINUTE);
  /*15*/ axAllIncNull[i++] = TypeId::xIdOf(IV_DAY_TO_SECOND);
  /*16*/ axAllIncNull[i++] = TypeId::xIdOf(IV_HOUR_TO_MINUTE);
  /*17*/ axAllIncNull[i++] = TypeId::xIdOf(IV_HOUR_TO_SECOND);
  /*18*/ axAllIncNull[i++] = TypeId::xIdOf(IV_MINUTE_TO_SECOND);
  /*19*/ axAllIncNull[i++] = TypeId::xIdOf(CT_TIME);
  /*20*/ axAllIncNull[i++] = TypeId::xIdOf(CT_TIMESTAMP);
  /*21*/ axAllIncNull[i++] = TypeId::xIdOf(CT_VARCHAR);
  /*22*/ axAllIncNull[i++] = TypeId::xIdOf(CT_UNKNOWN);
  ASSERT(i == sizeof(axAllIncNull)/sizeof(axAllIncNull[0]));

  bInitialised = true;
}

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

  vInitStaticArrays();

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
  _precondition_(_MAGIC_,CN_TYPEMAP1D,_FOOTER_);

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

  if (!xArg1.bIsValid()) {
    return TypeId{};
  }

  ASSERT(xArg1.iIdVal() >= 0 && xArg1.iIdVal() < m_iSize);

  i1 iResultId = m_aiTypeArray[xArg1.iIdVal()];
  return TypeId{iResultId};
}

/*----------------------------------------------------------------------------*/
/* Print the mappings that are set                                            */
/*----------------------------------------------------------------------------*/
void TypeMap1D::vPrintMap()
{
  s1M sBuf;
  sBuf.vConcat("TypeMap1D:\n\n"_Stz);


  for (i8 i=0; i<m_iSize; i++) {

    i1 iResult = m_aiTypeArray[i];
    TypeId xResult{iResult};

    if (xResult.bIsValid()) {
      TypeId xArg1{i};

      sBuf.vCatSprint("  %26s --> %s\n"_Stz, xArg1.szTypeName(), xResult.szTypeName());
    }
  }

  vTrace(sBuf.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
