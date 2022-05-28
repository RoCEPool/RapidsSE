/*============================================================================*/
/* TypeId.hpp - A class that represents an indirect reference to a type       */
/*============================================================================*/

#include "BASE.h"
#include "TypeId.hpp"
#include "TypeSystem.hpp"
#include "eIV.h"

using namespace Parser;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeId::TypeId(i8 iTypeId)
 : m_iId(iTypeId)
{
  // Note: not a proper object
}

/*----------------------------------------------------------------------------*/
/* equality operator                                                          */
/*----------------------------------------------------------------------------*/
b1 TypeId::operator==(const TypeId& rhs)
{
  return m_iId == rhs.m_iId;
}

/*----------------------------------------------------------------------------*/
/* inequality operator                                                        */
/*----------------------------------------------------------------------------*/
b1 TypeId::operator!=(const TypeId& rhs)
{
  return !(*this == rhs);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Check that this is a valid type ID                                         */
/*----------------------------------------------------------------------------*/
b1 TypeId::bIsValid()
{
  return TypeSystem::bIsValid(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the internal ID value, e.g. for printing                               */
/*----------------------------------------------------------------------------*/
i8 TypeId::iIdVal()
{
  return m_iId;
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of the parent of this type                                  */
/*----------------------------------------------------------------------------*/
TypeId TypeId::xParentTypeId()
{
  ASSERT(bIsValid());

  return TypeSystem::xParentTypeId(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the eCT value of the parent of this type. May be CT_NONE.              */
/*----------------------------------------------------------------------------*/
eCT TypeId::eCTVal()
{
  return TypeSystem::eCTVal(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the eIV value of the parent of this type. May be IV_NONE.              */
/*----------------------------------------------------------------------------*/
eIV TypeId::eIVVal()
{
  return TypeSystem::eIVVal(*this);
}

/*----------------------------------------------------------------------------*/
/* Return true if this type is an abstract type and cannot be instantiated.   */
/*----------------------------------------------------------------------------*/
b1 TypeId::bIsAbstract()
{
  ASSERT(bIsValid());

  return TypeSystem::bIsAbstract(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the namespace to which this type belongs.                              */
/*----------------------------------------------------------------------------*/
Stz* TypeId::szNamespace()
{
  ASSERT(bIsValid());

  return TypeSystem::szNamespace(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the name of this type (unique within its namespace).                   */
/*----------------------------------------------------------------------------*/
Stz* TypeId::szTypeName()
{
  ASSERT(bIsValid());

  return TypeSystem::szTypeName(*this);
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given eCT type.                                        */
/* Should not be used for interval types. Instead use iIdOf(eIV).             */
/*----------------------------------------------------------------------------*/
TypeId TypeId::xIdOf(eCT eVal)
{
  return TypeSystem::xIdOf(eVal);
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given eIV type.                                        */
/*----------------------------------------------------------------------------*/
TypeId TypeId::xIdOf(eIV eVal)
{
  return TypeSystem::xIdOf(eVal);
}

/*----------------------------------------------------------------------------*/
/* Get the typeId of a given namespace and typename. Currently a slow search. */
/* Returns an invalid TypeId if not found (check bIsValid()).                 */
/*----------------------------------------------------------------------------*/
TypeId TypeId::xIdOf(Stz* szNamespace, Stz* szTypeName)
{
  return TypeSystem::xIdOf(szNamespace, szTypeName);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
