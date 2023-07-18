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
  _nonProper_

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
  _nonProper_

  if (m_pText) {
    //RASSERT(CN_STZVAL == this->eType());
    delete m_pText;
    m_pText = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as LocalVar*                                    */
/*----------------------------------------------------------------------------*/
LocalVar::operator LocalVar*(void)
{
  _nonProper_

  return this;
}


/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
char* LocalVar::azArea(void)
{
  _nonProper_

  return m_axLocalStr->azArea();
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* LocalVar::szArea(void)
{
  _nonProper_

  return m_axLocalStr->szArea();
}

/*----------------------------------------------------------------------------*/
/* get discard point                                                          */
/*----------------------------------------------------------------------------*/
void* LocalVar::aDiscardPoint(void)
{
  _nonProper_

  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* get localStr                                                               */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStr(void)
{
  _nonProper_

  return m_axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* allot a localStr                                                           */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVar::axLocalStrAllot(void)
{
  _nonProper_

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
  _nonProper_

  return m_i_LocalVar;
}


/*----------------------------------------------------------------------------*/
/* revert a localStr                                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vLocalStrRevert(void)
{
  _nonProper_

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
  _nonProper_

  i8 nBytes;

  if (CN_STZVAL == eType()) {
    i8 jStz = jStzSize();
    i8 jSizeStz = sizeof(i8) + jStz;

    nBytes = sizeof(_Header) + jSizeStz;
  }
  else {
    nBytes = sizeof(TypeValue);
  }

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get string size                                                            */
/*----------------------------------------------------------------------------*/
i8 LocalVar::jStzSize(void)
{
  _nonProper_

  return m_axLocalStr->jStrSize();
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
eCN LocalVar::eType(void)
{
  _nonProper_

  return m_xTypeValue.m_wxType.eGetClassNo();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
_Header* LocalVar::axType(void)
{
  _nonProper_

  return m_xTypeValue.axType();
}

/*----------------------------------------------------------------------------*/
/* get pointer to type value                                                  */
/*----------------------------------------------------------------------------*/
TypeValue* LocalVar::axTypeValue(void)
{
  _nonProper_

  return & m_xTypeValue;
}

/*----------------------------------------------------------------------------*/
/* get pointer to type                                                        */
/*----------------------------------------------------------------------------*/
Dtm* LocalVar::axValue(void)
{
  _nonProper_

  return m_xTypeValue.axValue();
}


/*----------------------------------------------------------------------------*/
/* set local variable for given typeValue                                     */
/*----------------------------------------------------------------------------*/
void LocalVar::vSet(TypeValueArea* aTypeValueArea)
{
  _nonProper_

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
  _nonProper_

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

  eType_local = this->eType();
  ASSERT(CN_STZVAL == eType_local);
  ASSERT(m_pText);
}

/*----------------------------------------------------------------------------*/
/* set text from given typeValue                                              */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetText(TypeValueArea* aTypeValueArea)
{
  _nonProper_

  eCN eType_stack = aTypeValueArea->eType();
  ASSERT(CN_STZVAL == eType_stack);

  vSetType(aTypeValueArea);

  ASSERT(m_pText);

  LocalStr* axLocalStr = this->axLocalStr();

  SizeArea* axSizeArea = axLocalStr->axSizeArea();
  byte* ayByte = reinterpret_cast<byte*>(aTypeValueArea->axValue());
  i8 nBytes = aTypeValueArea->nStackUsage() - sizeof(_Header);

  MemLib::vCopy(axSizeArea,
                ayByte,
                nBytes,
                CN_STACK);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pText->vSet(reinterpret_cast<Stz*>(ayByte));
}

/*----------------------------------------------------------------------------*/
/* set 8B value from given typeValue                                          */
/*----------------------------------------------------------------------------*/
void LocalVar::vSetValue(TypeValueArea* aTypedValueArea)
{
  _nonProper_

  eCN eType_stack = aTypedValueArea->eType();
  ASSERT(CN_STZVAL != eType_stack);

  eCN eType_local = this->eType();

  if (CN_STZVAL == eType_local) {
    ASSERT(m_pText);
    this->vLocalStrRevert();
   }
  else {
    //ASSERT(! m_pText);
  }

  if (m_pText) {
    delete m_pText;
    m_pText = nullptr;
  }


  ASSERT(! m_pText);

  TypeValue* axTypeValue = this->axTypeValue();

  MemLib::vCopy(axTypeValue,
                aTypedValueArea,
                sizeof(TypeValue),
                CN_STACK);

  eType_local = this->eType();
  ASSERT(CN_STZVAL != eType_local);
  ASSERT(! m_pText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
