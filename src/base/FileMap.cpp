/*============================================================================*/
/* FileMap.cpp - map of page file on disk                                     */
/*============================================================================*/

#include "BASE.h"
#include "FileMap.hpp"
#include "CmdLineArgs.hpp"

//extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;
extern CmdLineArgs  gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FileMap* BASE::_pFileMap(void* aAddr)
{
  FileMap* pFileMap = reinterpret_cast<FileMap*>(aAddr);

  RASSERT_TYPE(pFileMap,CN_FILEMAP);

  return pFileMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FileMap::operator new(size_t)
{
  FileMap* pFileMap = (FileMap*)gpBufferPool->pAllocOtherBuffer();

  return pPoolCheck<void*>(pFileMap,__func__);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::operator delete(void* pFileMapAddress)
{
  FileMap* pFileMap = reinterpret_cast<FileMap*>(pFileMapAddress);

  Heaps::vFree(pFileMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FileMap::FileMap(void)
    : PoolBuffer(CN_FILEMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);
  m_szFilePath = gxCmdLineArgs.m_xSysArgs.m_sPageFilePath;

  vInitialise();

  m_pPageFile = new PageFile(m_szFilePath);

  _checkInitialised_(CN_FILEMAP);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FileMap::~FileMap(void)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* initialise all buffers on disk                                             */
/*----------------------------------------------------------------------------*/
void FileMap::vInitialise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  bool bProgress = false, bTouch = false;
  i8 iBufferMod;

  i8 i_PoolBuffer = gpBufferPool->m_nNonPageableBuffers;
  // Pickup initialization of the pageable buffers from, where BufferPool left off.
  // From m_nNonPageableBuffers...m_nBuffers-1

  while (C_NIL != i_PoolBuffer) {

    if (bProgress) {
      iBufferMod = i_PoolBuffer % 1000;
      if (! iBufferMod) {
        fflush(stdout);
      }
    }

    FreePoolBuffer* pFreePoolBuffer = gpBufferPool->pToFreePoolBuffer(i_PoolBuffer);

    pFreePoolBuffer->vSetMagic();
    pFreePoolBuffer->vSetClassNo(CN_FREEPOOLBUFFER);

    //pFreePoolBuffer->aFooter()->vSetFooter();
    if (bTouch)
      pFreePoolBuffer->vZeroData();

    pFreePoolBuffer->vSetFooter();
    pFreePoolBuffer->vBufferNo(i_PoolBuffer);
    i_PoolBuffer = gpBufferPool->iBufferNext(i_PoolBuffer);
    pFreePoolBuffer->vNextFree(i_PoolBuffer);

    gpBufferPool->m_nFreeCount++;
  }

  FreePoolBuffer* pLastBuffer = gpBufferPool->pToFreePoolBuffer(gpBufferPool->iBufferLast());

  pLastBuffer->vSetMagic();
  pLastBuffer->vSetClassNo(CN_FREEPOOLBUFFER);
  //pLastBuffer->aFooter()->vSetFooter();
  if (bTouch)
    pLastBuffer->vZeroData();

  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(gpBufferPool->iBufferLast());
  pLastBuffer->vNextFree(C_NIL);


  //vDump();
}

/*----------------------------------------------------------------------------*/
/* Decide where on disk this operand should be written to and write to disk   */
/*----------------------------------------------------------------------------*/
void FileMap::vToDisk(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  //---------------------------------------------------------------------------
  // Operand could be an address or a buffer id. Check flag bit.
#ifdef WITH_BUFFER_ID
  BufferId oBufferId;// TODO Arjun: Code readability
  oBufferId.m_pvAsAddress = (void*) pOperand;

  if(0 == BufferId::bIsAddress(oBufferId)) {
    pOperand = (Operand*) BufferId::vAddressFromId(oBufferId);//hashmap
  }

  //---------------------------------------------------------------------------
  // Look at page no bits in buffer id and assign to corresponding PageFile

    // To do once we have multiple page files...
    // m_pPageFile[oBufferId.m_uPageFile];

  //---------------------------------------------------------------------------
  // Write to page file

  RowBuffer* pRowBuffer = _pRowBuffer(pOperand);

  m_pPageFile->vWriteDataBufferToPFile(pRowBuffer);
#endif
}

/*----------------------------------------------------------------------------*/
/* translate pool buffer number to allocated buffer                           */
/*----------------------------------------------------------------------------*/
PoolBuffer* FileMap::pToPoolBuffer(i8 i_PoolBuffer)
{
  _static_

  i8 iOffset = C_SIZE_POOL_BUFFER * i_PoolBuffer;

  PoolBuffer* pPoolBuffer = static_cast<PoolBuffer*>(MemLib::aAddBytes(gpBufferPool->m_pBase,iOffset));

  return pPoolCheck<PoolBuffer*>(pPoolBuffer,__func__);
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FileMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);

  return &m_xFooter;
}