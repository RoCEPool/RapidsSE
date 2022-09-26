/*============================================================================*/
/* TypeMap1D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/

#include "BASE.h"
#include "TypeMap1D.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

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
