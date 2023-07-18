/*============================================================================*/
/* LocalVar.cpp -- Size + Area                                                */
/*============================================================================*/

#include "LocalVar.hpp"
#include "LocalStr.hpp"
#include "LocalVars.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalVar::LocalVar(void)
{
  m_pLocalVars    = nullptr;
  m_i_LocalVar    = C_UNKNOWN;
  m_aDiscardPoint = nullptr;
  //m_xTypeValue
  m_axLocalStr    = nullptr;

  m_pText         = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalVar::~LocalVar(void) noexcept(false)
{
  if (m_pText)
    delete m_pText;
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as LocalVar*                                    */
/*----------------------------------------------------------------------------*/
LocalVar::operator LocalVar*(void)
{
  return this;
}


/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
char* LocalVar::azArea(void)
{
  return m_axLocalStr->azArea();
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* LocalVar::szArea(void)
{
  return m_axLocalStr->szArea();
}

/*----------------------------------------------------------------------------*/
/* get discard point                                                          */
/*----------------------------------------------------------------------------*/
void* LocalVar::aDiscardPoint(void)
{
  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* get localStr                                                               */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStr(void)
{
  return m_axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* allot a localStr                                                           */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStrAllot(void)
{
  //vTrace("LocalVar: %lld StrAllot\n"_Stz,m_i_LocalVar);
  
  RASSERT(! m_axLocalStr);
  m_axLocalStr = m_pLocalVars->axLocalStrAllot();
  
  m_xTypeValue.m_wxType.vSetClassNo(CN_STZVAL);
  
  return m_axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* get local                                                                  */
/*----------------------------------------------------------------------------*/
i8 LocalVar::i_LocalVar(void)
{
  return m_i_LocalVar;
}


/*----------------------------------------------------------------------------*/
/* revert a localStr                                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vLocalStrRevert(void)
{
  //vTrace("LocalVar: %lld StrRevert\n"_Stz,m_i_LocalVar);
  
  m_pLocalVars->vLocalStrRevert(m_axLocalStr);
  m_axLocalStr = nullptr;
  
  m_xTypeValue.m_wxType.vSetClassNo(CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* get stack usage                                                            */
/*----------------------------------------------------------------------------*/
i8 LocalVar::nStackUsage(void)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* get string size                                                            */
/*----------------------------------------------------------------------------*/
i8 LocalVar::jStzSize(void)
{
  return m_axLocalStr->jStrSize();
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
eCN LocalVar::eType(void)
{
  return m_xTypeValue.m_wxType.eGetClassNo();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
_Header* LocalVar::axType(void)
{
  return m_xTypeValue.axType();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type value                                                  */
/*----------------------------------------------------------------------------*/
TypeValue* LocalVar::axTypeValue(void)
{
  return & m_xTypeValue;
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
Dtm* LocalVar::axValue(void)
{
  return m_xTypeValue.axValue();
}


/*----------------------------------------------------------------------------*/
/* set local variable for given typeValue                                     */
/*----------------------------------------------------------------------------*/
void LocalVar::vSet(TypeValueArea* aTypeValueArea)
{
  eCN eType_stack = aTypeValueArea->eType();

  if (CN_STZVAL == eType_stack)
    vSetText(aTypeValueArea);
  else
    vSetValue(aTypeValueArea);
}


/*----------------------------------------------------------------------------*/
/* set type of local variable from given typeValue                            */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetType(TypeValueArea* aTypeValueArea)
{
  eCN eType_stack = aTypeValueArea->eType();
  eCN eType_local = this->eType();

  ASSERT(CN_STZVAL == eType_stack);

  LocalStr *axLocalStr;
  if (CN_STZVAL == eType_local)
    axLocalStr = this->axLocalStr();
  else
    axLocalStr = this->axLocalStrAllot();

  if (! m_pText)
    m_pText = new Text;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MemLib::vCopy(axType(),
                aTypeValueArea,
                sizeof(_Header),
                CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* set text from given typeValue                                              */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetText(TypeValueArea* aTypeValueArea)
{
  eCN eType_stack = aTypeValueArea->eType();
  ASSERT(CN_STZVAL == eType_stack);

  vSetType(aTypeValueArea);

  LocalStr* axLocalStr = this->axLocalStr();

  SizeArea* axSizeArea = axLocalStr->axSizeArea();
  byte* ayByte = reinterpret_cast<byte*>(aTypeValueArea->axValue());
  i8 nBytes = aTypeValueArea->nStackUsage() - sizeof(_Header);

  MemLib::vCopy(axSizeArea,
                ayByte,
                nBytes,
                CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* set 8B value from given typeValue                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetValue(TypeValueArea* aTypedValueArea)
{
  eCN eType_stack = aTypedValueArea->eType();
  ASSERT(CN_STZVAL != eType_stack);

  eCN eType_local = this->eType();

  if (CN_STZVAL == eType_local) {
    this->vLocalStrRevert();
    if (m_pText)
      delete m_pText;
  }

  TypeValue* axTypeValue = this->axTypeValue();

  MemLib::vCopy(axTypeValue,
                aTypedValueArea,
                sizeof(TypeValue),
                CN_STACK);

  eType_local = this->eType();
  ASSERT(CN_STZVAL != eType_local);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
