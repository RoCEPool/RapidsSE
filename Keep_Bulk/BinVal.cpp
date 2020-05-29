/*============================================================================*/
/* BinVal.cpp - string value                                                  */
/*============================================================================*/

#include "Values.h"
#include "StorageEngine.h"

extern i8 giClassSize[];

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BinVal* Values::_pBinVal(void* aAddr)
{
  BinVal* pBinVal = reinterpret_cast<BinVal*>(aAddr);
  
  RASSERT_TYPE(pBinVal,CN_BINVAL);
  
  return pBinVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BinVal::operator new(size_t)
{
  vFatal("fatal: BinVal class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BinVal::operator new(size_t, BinVal* pBinVal)
{
  return pBinVal;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BinVal::BinVal(void)
: Value(CN_BINVAL)
{
  m_nAreaSize = 0;
  
  _checkInitialised_(CN_BINVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BinVal::BinVal(i8 iChars)
: Value(CN_BINVAL)
{
  m_nAreaSize = iChars;
  
  _memZero_(pData(),m_nAreaSize,CN_BINVAL);
  
  _checkInitialised_(CN_BINVAL);
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void BinVal::operator =(Object* pObject)
{
  m_nAreaSize = giClassSize[pObject->eClass()];
  
  MemLib::vCopy(m_cArea, pObject, m_nAreaSize, CN_BINVAL);
}

/*----------------------------------------------------------------------------*/
/* get iChars                                                                 */
/*----------------------------------------------------------------------------*/
i8 BinVal::iBytes(void) const
{
  return m_nAreaSize;
}

/*----------------------------------------------------------------------------*/
/* get data area                                                              */
/*----------------------------------------------------------------------------*/
char* BinVal::pData(void)
{
  return &m_cArea[0];
}

/*----------------------------------------------------------------------------*/
/* get pObject class                                                          */
/*----------------------------------------------------------------------------*/
eCN BinVal::eObjectClass(void)
{
  Object* pBinaryObject = (Object*)pData();
  
  return pBinaryObject->eClass();
}

/*----------------------------------------------------------------------------*/
/* reset string value                                                         */
/*----------------------------------------------------------------------------*/
void BinVal::vReset(void)
{
  m_nAreaSize = 0;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 BinVal::nSize(void) const
{
  i8 iOverrun = m_nAreaSize > 8 ? m_nAreaSize - 8 : 0;
  
  i8 iSize = sizeof(BinVal) + iOverrun;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void BinVal::vSize(void)
{
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void BinVal::vSize(i8 iSize)
{
  m_nAreaSize = iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void BinVal::vToString(Stz* aWorkspace)
{
  Object* pEmbedded = (Object*)pData();
  
  aWorkspace->vSprint("%p BinVal [%s of %lld bytes]"_Stz,
                      this,
                      aCN[pEmbedded->eClass()],
                      m_nAreaSize);
}

/*----------------------------------------------------------------------------*/
/* get new object from binary                                                 */
/*----------------------------------------------------------------------------*/
Object* BinVal::pNewObject(void)
{
  ASSERT(CN_BINVAL == eClass());
  
  //BinVal* pBinVal = reinterpret_cast<BinVal*>(this);
  
  i8 iSize = this->nSize();
  Object* pObject = HeapMem::pAllocate(eObjectClass());
  MemLib::vCopy(pObject, pData(), iSize, CN_BINVAL);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes reqired to hold object of given length           */
/*----------------------------------------------------------------------------*/
i8 BinVal::nBytesRequired(i8 iLength)
{
  i8 nBytes = sizeof(BinVal) + iLength + 1 - 8;
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
