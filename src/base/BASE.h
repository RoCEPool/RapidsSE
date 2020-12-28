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

#define EndDefine

#define _static_ EndDefine
#define _nonProper_ EndDefine
#define _nonLocalStatic_ EndDefine
#define _outsideOfPool_ EndDefine

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"
#include "UsefulConstants.h"
#include "UserStrSizes.h"
#include "Namespaces.h"

//------------------------------------------------------------------------------

namespace BASE {

  template <i8 BYTES> class String_t;
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

  template <int BYTES> class Bytes_t;
  typedef Bytes_t<32>      b32;
  typedef Bytes_t<256>     b256;
  typedef Bytes_t<C_4K>    b4K;   // note: K, not KB
  typedef Bytes_t<C_8K>    b8K;   // note: K, not KB
  typedef Bytes_t<C_32K>   b32K;  // note: K, not KB
  typedef Bytes_t<C_256K>  b256K; // note: K, not KB
}

namespace BASE       { class ForthCell;            class Focal;
                              class Header;          class Object;
                              class Statistics;     class Mutex;              }
namespace BASE            { class ReqTuple;                              }
namespace BASE           { class Lock;            class LockTracker;       }
namespace BASE              { class BufferPool;      class HeapBuffer;
                              class HeapMem;         class PoolBuffer;

class IvlLib;

}


using namespace BASE;


//------------------------------------------------------------------------------
#include "AxiomaticConstants.h"
#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "CommonTypes.h"
#include "EnvFlags.h"
#include "Gid.h"
#include "Nibbler.h"
#include "Null.hpp"
#include "WordAsBytes.h"


#if !( (defined(WITH_APPLE) || defined(WITH_LINUX)) )
  #include "Asm.h"
#endif

#include "Asm.h"

#include "PmcUse.h"


#include "Stz.hpp"
#include "StrLib.hpp"



#include "ClassNumbers.hpp"
#include "FnDesc.hpp"
#include "FnDefn.hpp"
#include "FnCatalog.hpp"
#include "LimitsFormats.h"

#include "AM.hpp"
#include "eHT.h"

struct SizeHeap {
  
  i8  m_nClassSize;
  eHT m_eWhichHeap;
  
  i8  m_nVtableAdjust; // temp: explore support for vtable

};



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

namespace BASE { class TokenVec; }
typedef void (fpSELFFN)(TokenVec* pTokenVec);
#include "SelfFn.hpp"
#include "SelfCatalog.hpp"

#include "SubSysFn.hpp"
#include "SubSysCatalog.hpp"

#include "GizmoFn.hpp"
#include "GizmoCatalog.hpp"

#include "ToolFn.hpp"
#include "ToolCatalog.hpp"


namespace Expressions {
  typedef void (Step::*StepFp)(void);
}


#include "ForthCell.hpp"


//typedef void (pDUMPFUN)(eCN eClass, Object* pObject, Stz*);
//extern pDUMPFUN *gpDump[];

//typedef void (pOUTLINEFUN)(Focal* pFocal, Stz*);
//extern pOUTLINEFUN *gpOutline[];



#include "Bytes_t.hpp"
#include "String_t.hpp"

#include "SP.hpp"
#include "SPmedium.hpp"
#include "SPsmall.hpp"




#include "FocalDesc.hpp"

#include "NamedObject.hpp"
#include "ReqTuple.hpp"
#include "SP_Gid.hpp"


#include "IntLib.hpp"
#include "MagLib.hpp"
#include "Dtm.hpp"
#include "TypedCell.hpp"
#include "HshLib.hpp"

#include "SP_Bits.hpp"

//#include "Compress.hpp"
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

#include "SP_Lock.hpp"
#include "SP_Mutex.hpp"


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














namespace BASE
{

  void vCheckAssumptions(void);
}

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

#include "SP_AggTimer.hpp"
#include "SP_Bytes.hpp"
#include "SP_Char.hpp"
#include "SP_Elapsed.hpp"
#include "SP_Footer.hpp"
#include "SP_Header.hpp"
#include "SP_ReqTuple.hpp"
#include "SP_String.hpp"
#include "SP_TagSet.hpp"
#include "SP_Timer.hpp"

typedef void (*FNADDR)(void*);

#define INTRINSIC(FNNAME) \
  \
  FnDesc FnDesc_ ## FNNAME (#FNNAME ## _Stz, (FNADDR)Step::FNNAME); \
  \
  void Step::FNNAME(Forth* pForth) \
EndDefine

#define JOBS_fn(FNNAME) \
  \
  FnDesc FnDesc_ ## FNNAME (#FNNAME ## _Stz, (FNADDR)Arguments::FNNAME); \
  \
  void Arguments::FNNAME(Job* pJob) \
EndDefine














#include "RU.hpp"
#include "RU_Cpu.hpp"
#include "RU_Memory.hpp"







extern BufferPool* gpBufferPool;



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

typedef void (pARTFUN)(Object* pObject, SP& spObject);

#include "Article.hpp"
#include "MiniHeap_t.hpp"
#include "TrayNL_t.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Article.hpp"




#include "Dir.hpp"    //?
#include "Fifo.hpp"   //?
#include "File.hpp"   //?
#include "PsxThd.hpp" //?
#include "Socket.hpp" //?

#include "TlsKeys.hpp" //?

#include "ResourceTracker.hpp" //?


#include "AnnChar.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"

#include "Charser.hpp"
#include "Input.hpp"

#include "SP_AnnChar.hpp"
#include "SP_Charser.hpp"
#include "SP_Token.hpp"
#include "SP_TknVecStr.hpp"




#include "ShmUtility.hpp"




#include "Thing.hpp"
#include "Arrange.hpp"
#include "IntSet.hpp"




namespace BASE
{
  static void vSubSystem(TokenVec* pTokenVec);
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
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
