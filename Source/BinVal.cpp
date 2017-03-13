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
  
  RASSERT( !pBinVal || CN_BINVAL == pBinVal->eClass() );
  
  return pBinVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BinVal::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: BinVal class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BinVal::operator new(size_t iSize, BinVal* pBinVal)
{
  RASSERT(iSize);

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
  
  MemLib::vZero(pData(),m_nAreaSize);
  
  _checkInitialised_(CN_BINVAL);
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void BinVal::operator =(const Object* pObject)
{
  m_nAreaSize = giClassSize[pObject->eClass()];
  
  MemLib::vCopy(m_cArea, pObject, m_nAreaSize);
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
  ASSERT(0);
  //m_nAreaSize = StrLib::iSize(m_sStr);
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
void BinVal::vToString(Str* aWorkspace)
{
  Object* pEmbedded = (Object*)pData();
  
  aWorkspace->vSprint("%p BinVal [%s of %lld bytes]"_Str,
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
  Object* pObject = Heap::pAllocate(eObjectClass());
  MemLib::vCopy(pObject, pData(), iSize);
  
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
