/*============================================================================*/
/* BASE.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//------------------------------------------------------------------------------

namespace BASE
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include <iostream>
#include <string.h>
//typedef unsigned long size_t;

//------------------------------------------------------------------------------

#define EndDefine

#define _static_ EndDefine
#define _nonProper_ EndDefine
#define _nonLocalStatic_ EndDefine
#define _outsideOfPool_ EndDefine

#include "CompiledWith.h"

//------------------------------------------------------------------------------

//#include "MachineTypes.h"

namespace BASE
{
  typedef unsigned char      b1;   // 1 byte boolean
  typedef unsigned short     b2;   // 2 byte boolean
  typedef unsigned long      b4;   // 4 byte boolean
  typedef unsigned long long b8;   // 8 byte boolean
  
  typedef float              d4;   // 4 byte float
  typedef double             d8;   // 8 byte float
  typedef long double        d10;  // 10 byte float

  typedef char               i1;   // 1 byte signed integer
  typedef short              i2;   // 2 byte signed integer
  typedef int                i4;   // 4 byte signed integer
  typedef long long          i8;   // 8 byte signed integer
  typedef __int128           i16;  // 16 byte signed integer (GNU extension)

  typedef unsigned char      m1;   // 1 byte memory cell
  typedef unsigned short     m2;   // 2 byte memory cell
  typedef unsigned long      m4;   // 4 byte memory cell
  typedef unsigned long long m8;   // 8 byte memory cell

  typedef i8                 t8;   // 8 byte transaction perspective

  typedef unsigned char      u1;   // 1 byte unsigned integer
  typedef unsigned short     u2;   // 2 byte unsigned integer
  typedef unsigned int       u4;   // 4 byte unsigned integer
  typedef unsigned long long u8;   // 8 byte unsigned integer
  typedef unsigned __int128  u16;  // 16 byte unsigned integer (GNU extension)

  typedef unsigned long      w4;   // 4 byte word
  typedef unsigned long long w8;   // 8 byte word

  typedef unsigned char      byte; // a byte
  typedef unsigned char      y1;   // 1 byte
  typedef unsigned short     y2;   // 2 byte
  typedef unsigned int       y4;   // 4 byte
  typedef unsigned long long y8;   // 8 byte
  
  typedef void           Opaque;   //

  const u8 C_BITS_PER_BYTE         = 8;
  const u8 C_BITS_PER_WORD         = 64;
  const u8 C_BYTES_PER_WORD        = 8;
  const u8 C_BYTES_PER_DOUBLE_WORD = 16;
  const u8 C_BYTES_PER_QUAD_WORD   = 32;
  const u8 C_BYTES_PER_CACHE_LINE  = 64;
  const u8 C_NIBBLES_PER_BYTE      = 2;

  const u8 C_WORDS_PER_WORD        = 1;
  const u8 C_WORDS_PER_DOUBLE_WORD = 2;
  const u8 C_WORDS_PER_QUAD_WORD   = 4;
  const u8 C_WORDS_PER_CACHE_LINE  = 8;

  struct _WordAsBytes {
    union {
      b1 m_yaByte[C_BYTES_PER_WORD];
      i8 m_iI8;
      u8 m_uU8;
    };
  };

}

//------------------------------------------------------------------------------

#include "UsefulConstants.h"

static const i8 C_MAX_RECORD    = C_16KB;       // max length of record
static const i8 C_MAX_STATEMENT = C_32KB - 64;  // max length of statement
static const i8 C_MAX_VCH       = C_4K;         // max length of vch value

static const i8 C_ALL          = -1;
static const i8 C_VARGS_BEGIN  = -1;
static const i8 C_VARGS_END    = -1;
static const i8 C_NEVER_THROWN = -1;
static const i8 C_NIL          = -1;
static const i8 C_UNKNOWN      = -1;
static const i8 C_IGNORED      = -1;

static const bool B_CALL_AGAIN      = true;
static const bool B_DONT_CALL_AGAIN = false;

static const b8 C_NONFOCAL          = true;

static const i8 C_SET_OFFSETS       = 128;
static const i8 C_LOAD_READCHUNK    = C_1M; // its 1M not 1MB (i.e. < 1MB)
static const i8 C_CHUNKBLOCK        = 2048;

//------------------------------------------------------------------------------

// initialised in IntLib::vInitGlobals()
extern i8 C_DATUM_NULL;
extern i8 C_DATUM_MINUS_INFINITY;
extern i8 C_DATUM_PLUS_INFINITY;

extern i8 C_I8_MIN;
extern i8 C_I8_MAX;
extern i8 C_I8_MID;

// initialised in FltLib::vInitGlobals()
extern d8 C_D8_MIN;
extern d8 C_D8_MAX;
extern d8 C_D8_MID;

//------------------------------------------------------------------------------

#include "BitLib.hpp"

static const u8 C_SIZE_POOL_BUFFER   = C_1MB;

static const i8 C_SIZE_TINY_OBJECT   = C_1KB;
static const i8 C_SIZE_SMALL_OBJECT  = C_4KB;
static const i8 C_SIZE_MEDIUM_OBJECT = C_64KB;
static const i8 C_SIZE_LARGE_OBJECT  = C_SIZE_POOL_BUFFER - C_SIZE_SMALL_OBJECT;

static const u8 C_MASK_TINY_OBJECT   = 0x3FF;
static const u8 C_MASK_SMALL_OBJECT  = 0xFFF;
static const u8 C_MASK_MEDIUM_OBJECT = 0xFFFF;
static const u8 C_MASK_LARGE_OBJECT  = 0xFFFFF;

static const i8 C_LOW_BUFFERS        = 512;

static const u8 C_OBJECT_TO_BUFFER_MASK = BITWISE_NOT((C_SIZE_POOL_BUFFER - 1));

static const i8 C_VIA_MALLOC         = 1;

//------------------------------------------------------------------------------

namespace BASE
{
  template <i8 AREA> class String_t;
  typedef String_t<32>     s32;
  typedef String_t<256>    s256;
  typedef String_t<C_2K>   s2K;   // note: 2K not 2KB
  typedef String_t<C_4K>   s4K;   // note: 4K not 4KB
  typedef String_t<C_16KB> s16KB; // note: this is 16KB
  typedef String_t<C_24KB> s24KB; // note: this is 17KB
  typedef String_t<C_48K>  s48K;  // note: 48K not 24KB
  typedef String_t<C_1M>   s1M;   // note: 1M not 1MB

  typedef String_t<C_MAX_STATEMENT> sSTMT;
  typedef String_t<C_MAX_VCH>       sVCH;

  template <i8 AREA> class Bytes_t;
  typedef Bytes_t<32>      b32;
  typedef Bytes_t<256>     b256;
  typedef Bytes_t<C_4K>    b4K;   // note: K, not KB
  typedef Bytes_t<C_8K>    b8K;   // note: K, not KB
  typedef Bytes_t<C_32K>   b32K;  // note: K, not KB
  typedef Bytes_t<C_256K>  b256K; // note: K, not KB
}

//------------------------------------------------------------------------------

#include "ByteLiterals.hpp"
#include "CommonTypes.h"
#include "EnvFlags.h"
#include "Gid.h"
#include "Null.hpp"

#include "Asm.h"

#include "Stz.hpp"
#include "StrLib.hpp"

#include "ClassNumbers.hpp"
#include "SizeHeap.h"
#include "LimitsFormats.h"

#include "FnDesc.hpp"
#include "FnDefn.hpp"
#include "FnCatalog.hpp"

#include "AM.hpp"

//------------------------------------------------------------------------------

#include "Hack.hpp"

#define PASTE3(a,b,c) a##b##c
#define GLOBALNM(_fun,_class,_line) PASTE3(_fun,_class,_line)
#define HACK(_class,_text) Hack GLOBALNM(Hack,_class,__LINE__)(_text,__FILE__,__LINE__)

#include "eFK.h"
#include "Exception.hpp"

#include "Footer.hpp"
#include "Header.hpp"

#include "Correct.hpp"

#include "Fault.hpp"

#include "MemLib.hpp"

#include "AssertMacros.h"

#include "Object.hpp"
#include "StackObject.hpp"
#include "NonProper.hpp"

#include "Addressing.hpp"
#include "SizedHeader.hpp"
#include "Trace.hpp"
#include "Jot.hpp"
#include "IntLib.hpp"
#include "FltLib.hpp"

#include "Statistics.hpp"
#include "Focal.hpp"

#include "String.hpp"

#include "PTR.hpp"
#include "SMA.hpp"
#include "REF_t.hpp"
#include "REG_t.hpp"

typedef bool (fpCLASSFN)(Object* pObject);
#include "ClassFn.hpp"
#include "ClassCatalog.hpp"

namespace BASE { class TokenVec; class SelfFn; }
typedef void (fpSELFFN)(SelfFn* aSelfFn, TokenVec* pTokenVec);
#include "SelfFn.hpp"
#include "SelfCatalog.hpp"

#include "SubSysFn.hpp"
#include "SubSysCatalog.hpp"

#include "GizmoFn.hpp"
#include "GizmoCatalog.hpp"

#include "ToolFn.hpp"
#include "ToolCatalog.hpp"

#include "CodeCell.hpp"

#include "Bytes_t.hpp"
#include "String_t.hpp"

#include "SP.hpp"
#include "SPmedium.hpp"
#include "SPsmall.hpp"

#include "FocalDesc.hpp"

#include "NamedObject.hpp"
#include "ReqTuple.hpp"

#include "IntLib.hpp"
#include "MagLib.hpp"
#include "Dtm.hpp"
#include "TypedCell.hpp"
#include "CellSingle.hpp"

#include "HshLib.hpp"

#include "IvlLib.hpp"
#include "RndLib.hpp"
#include "TspLib.hpp"

#include "SysCall.hpp"

#include "Env.hpp"
#include "Hardware.hpp"
#include "HwCounters.hpp"
#include "OpSys.hpp"
#include "Process.hpp"
#include "Shell.hpp"

#include "Atomic.hpp"
#include "Lock.hpp"
#include "LockTracker.hpp"
#include "Mutex.hpp"

#include "SingleWord.hpp"
#include "AlignedObject.hpp"

#include "Line.hpp"
#include "DoubleWord.hpp"
#include "QuadWord.hpp"
#include "CacheLine.hpp"
#include "SpaceUnit.hpp"

#include "WspConvergent_t.hpp"
#include "Wsp_t.hpp"

#include "ShmLib.hpp"

//------------------------------------------------------------------------------

#include "PoolBuffer.hpp"
#include "FreeBuffer.hpp"
#include "FreeChunk.hpp"
#include "BufferPool.hpp"
#include "PoolHelper.hpp"
#include "PersistenceHelper.hpp"
#include "ResourceHelper.hpp"

#include "HeapBuffer.hpp"
#include "HeapMem.hpp"
#include "Heap.hpp"
#include "ObjectSnapshot.hpp"
#include "Region.hpp"
#include "Slab.hpp"
#include "SlabAllocator.hpp"

//------------------------------------------------------------------------------

#include "Birth.hpp"
#include "CRM.hpp"
#include "DecaCnt.hpp"
#include "Decider.hpp"
#include "EnumFlags.hpp"
#include "FreqDist.hpp"
#include "HV.hpp"
#include "MemTest.hpp"
#include "NextGid.hpp"
#include "Now.hpp"
#include "Rand.hpp"
#include "SeeMe.hpp"
#include "Sleep.hpp"
#include "Scan.hpp"
#include "Sprint.hpp"
#include "TagSet.hpp"
#include "TempName.hpp"
#include "TickingDevice.hpp"
#include "Timer.hpp"
#include "AggTimer.hpp"
#include "Gizmos.hpp"

#include "ByteRate.hpp"
#include "Peruse.hpp"
#include "Numbat.hpp"

#include "RU.hpp"
#include "RU_Cpu.hpp"
#include "RU_Memory.hpp"

#include "ArrayNL_t.hpp"

#include "FreeItem.hpp"

#include "HashInsert.hpp"
#include "HashEntry.hpp"
#include "HashBuffer.hpp"
#include "HashMap.hpp"
#include "HashCursor.hpp"

#include "LatticeNL_t.hpp"
#include "Lexicon.hpp"
#include "Link_t.hpp"
#include "ListItem_t.hpp"
#include "List_t.hpp"
#include "ListExo_t.hpp"
#include "ListNL_t__.hpp"
#include "ListNL_t.hpp"

#include "QueueBuffer.hpp"
#include "Queue.hpp"

#include "SkipNode.hpp"
#include "SkipListNL.hpp"
#include "StackNL_t.hpp"

#include "TextBuffer.hpp"
#include "TextLarge.hpp"
#include "TextMedium.hpp"
#include "TextSmall.hpp"
#include "TextTiny.hpp"
#include "Text.hpp"

#include "Vector_t.hpp"
#include "VectorNL_t.hpp"

#include "Wilderness.hpp"

// -- -- -- -- -- -- -- --

typedef void (fpARTICLEFN)(Object* pObject, SP& spObject);

#include "Article.hpp"
#include "MiniHeap_t.hpp"
#include "TrayNL_t.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Article.hpp"

#include "Dir.hpp"
#include "Fifo.hpp"
#include "File.hpp"
#include "PsxThd.hpp"
#include "Socket.hpp"

#include "TlsKeys.hpp"

#include "ResourceTracker.hpp"

#include "AnnChar.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"

#include "Charser.hpp"
#include "Input.hpp"

#include "ShmUtility.hpp"

#include "Thing.hpp"
#include "Arrange.hpp"
#include "IntSet.hpp"

#include "Format.hpp"

//------------------------------------------------------------------------------

#include "ColPix.h"
#include "RowPix.h"
#include "Column.hpp"

//------------------------------------------------------------------------------

#include "Literal.hpp"

#include "Licence.hpp"
#include "Limits.hpp"

//------------------------------------------------------------------------------

// declare a function pointer to any member function of Step
typedef void (Step::*fpSTEPFN)(void);

#include "Operand.hpp"

#include "Dic.hpp"
#include "DicStack.hpp"
#include "Local.hpp"
#include "LocalChain.hpp"
#include "CodeBlock.hpp"
#include "TypedStack.hpp"

#include "Compiler.hpp"
#include "Word.hpp"

#include "Profile.hpp"
#include "Task.hpp"

#include "TaskList.hpp"
#include "TaskQueue.hpp"

//------------------------------------------------------------------------------

#include "DataChunk.hpp"
#include "RecordChunk.hpp"
#include "FileChunk.hpp"

#include "IngestChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"

#include "OperandQueue.hpp"

#include "LinkedBuffer.hpp"

//------------------------------------------------------------------------------

#include "Tx.hpp"
#include "TxSet.hpp"

#include "Ref.hpp"
#include "DataBuffer.hpp"

#include "CsvMeta.hpp"
#include "CsvBuffer.hpp"
#include "Reader.hpp"

#include "CmdHelper.hpp"

#include "LogEntry.hpp"
#include "Log.hpp"

#include "ClassPlaceHolder.hpp"

//------------------------------------------------------------------------------

#include "Scheduler.hpp"

#include "Thread.hpp"
#include "ThreadQueue.hpp"
#include "ThreadVec.hpp"

#include "SystemSchedule.hpp"

//------------------------------------------------------------------------------

namespace BASE
{
  static void vSubSystem(SelfFn* aSelfFn, TokenVec* pTokenVec);
}

/*---------------------------------------------------------------------------*/
/* pCheck: check address                                                     */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
static OBJECT pPoolCheck(void* aAddress)
{
#ifdef WITH_ADDRESS_CHECKING
  if (aAddress)
    gpBufferPool->vCheckAddress(aAddress);
#endif
  
  return static_cast<OBJECT>(aAddress);
}

namespace BASE
{
  bool bTraceWith(Stz* szWith);
  void vTraceWith(void);

  bool bTraceWithout(Stz* szWith);
  void vTraceWithout(void);

  void vInitAllGlobals(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
