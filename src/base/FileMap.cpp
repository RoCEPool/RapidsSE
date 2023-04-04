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
 : Object(CN_FILEMAP),
 m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);
  m_szFilePath = gxCmdLineArgs.m_xSysArgs.m_sPageFilePath;

//  vInitialise();

  m_pPageFile = new PageFile(m_szFilePath, 0);

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
/* Find where on disk the data buffer is written, and get it                  */
/*----------------------------------------------------------------------------*/
DataBuffer* FileMap::pDataBufferFromDisk(BufferId oBufferId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  //---------------------------------------------------------------------------
  // Ensure id flag bit is set

  ASSERT(1 == BufferId::bIsBufferId(oBufferId));

  //---------------------------------------------------------------------------
  // Look at page no bits in buffer id and get to corresponding PageFile pointer

  // To do once we have multiple page files...
  // m_pPageFile[oBufferId.m_uPageFile];

  //---------------------------------------------------------------------------
  // Read from page file

  return m_pPageFile->pReadDataBufferFromPFile(oBufferId);
}

/*----------------------------------------------------------------------------*/
/* Decide where on disk this operand should be written to and write to disk   */
/*----------------------------------------------------------------------------*/
void FileMap::vToDisk(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  //---------------------------------------------------------------------------
  // Operand could be an address or a buffer id. Check flag bit.

//  BufferId oBufferId((void*)pOperand);
//
//  if(1 == BufferId::bIsBufferId(oBufferId)) {
//    vTrace("Writing BufferId %p to disk\n"_Stz, oBufferId.m_pvAsAddress);
//    pOperand = (Operand*) BufferId::aAddressFromId(oBufferId);//hashmap
//  }
//
//  //---------------------------------------------------------------------------
//  // Look at page no bits in buffer id and assign to corresponding PageFile
//
//    // To do once we have multiple page files...
//    // m_pPageFile[oBufferId.m_uPageFile];
//
//  //---------------------------------------------------------------------------
//  // Write to page file
//  RASSERT(CN_ROWBUFFER == pOperand->eGetClassNo() ||
//          CN_CRWBUFFER == pOperand->eGetClassNo());
//
//  DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
//
//  m_pPageFile->vWriteDataBufferToPFile(pDataBuffer);
}

/*----------------------------------------------------------------------------*/
/* Decide where on disk this buffer should be written to and write to disk    */
/*----------------------------------------------------------------------------*/
void FileMap::vToDisk(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  //---------------------------------------------------------------------------
  // Look at page no bits in buffer id and assign to corresponding PageFile

  // TODO: once we have multiple page files...
  // m_pPageFile[oBufferId.m_uPageFile];

  //---------------------------------------------------------------------------
  // Write to page file
  RASSERT(CN_ROWBUFFER == pDataBuffer->eGetClassNo() ||
          CN_CRWBUFFER == pDataBuffer->eGetClassNo());

  m_pPageFile->vWriteDataBufferToPFile(pDataBuffer);
}

#ifdef WITH_BUFFER_ID
/*----------------------------------------------------------------------------*/
/* Find a free chunk in a page file and return that address                   */
/*----------------------------------------------------------------------------*/
BufferId FileMap::oGetNewBufferId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  //---------------------------------------------------------------------------
  // Figure out which PageFile has space for a new RowBuffer

  // Single PageFile for now

  //---------------------------------------------------------------------------
  // Calculate the BufferId

  vGrabWriteLock();

  BufferId oFreeBufferId;

  u8 uFreeSubset = m_pPageFile->uNextFreeSubset();
  RASSERT(uFreeSubset != C_DATUM_NULL);

  oFreeBufferId.m_uSeekOffset = uFreeSubset;
  oFreeBufferId.m_uPageFile = m_pPageFile->m_uPFIndex;
  oFreeBufferId.m_bIdFlag = 1;

  vDropWriteLock();
  return oFreeBufferId;

//  return BufferId::vBufferIdFromNo(iPoolBufferNo);
}
#endif

/*----------------------------------------------------------------------------*/
/* translate pool buffer number to allocated buffer                           */
/*----------------------------------------------------------------------------*/
PoolBuffer* FileMap::pToPoolBuffer(i8 i_PoolBuffer)
{
  _static_

  i8 iOffset = C_SIZE_POOL_BUFFER * i_PoolBuffer;

  PoolBuffer* pPoolBuffer = static_cast<PoolBuffer*>(
      MemLib::aAddBytes(gpBufferPool->m_pBase,iOffset));

  return pPoolCheck<PoolBuffer*>(pPoolBuffer,__func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vGrabWriteLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vDropWriteLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vGrabReadLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vDropReadLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vStartPageIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  vGrabReadLock();

  if(! m_pPageFile->m_pFile->bIsOpen()) {
    m_pPageFile->m_pFile->bOpenMode(
        m_pPageFile->m_szPFilePath, C_OPEN_READ_WRITE_EXIST);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vEndPageIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);
  vDropReadLock();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vStartPageOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  vGrabWriteLock();

  if(! m_pPageFile->m_pFile->bIsOpen()) {
    m_pPageFile->m_pFile->bOpenMode(
        m_pPageFile->m_szPFilePath, C_OPEN_READ_WRITE_EXIST);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::vEndPageOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  m_pPageFile->m_pFile->vFlush();
  m_pPageFile->m_pFile->vSync();
  m_pPageFile->m_pFile->vClose();
}


/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FileMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/