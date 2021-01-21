/*============================================================================*/
/* Output.cpp -- Output                                                       */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "StreamSet.hpp"
#include "Session.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

#include "Grid.hpp"
#include "Response.hpp"
#include "Output.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Output,CN_OUTPUT)
__aNewFunction(Output,CN_OUTPUT)
__vDeleteFunction(Output,CN_OUTPUT)
__aFooterFunction(Output)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Output::Output(void)
: Object(CN_OUTPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
      
  m_pOutputQueue      = nullptr;

  _checkInitialised_(CN_OUTPUT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Output::~Output(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  if (m_pOutputQueue)
    delete m_pOutputQueue;
  m_pOutputQueue = nullptr;  
}

/*----------------------------------------------------------------------------*/
/* -- Public ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drain the result/output queue                                              */
/*----------------------------------------------------------------------------*/
void Output::vDrainBufferQueue(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);

  Operand* pOperand;

  while ( (pOperand = pPopBufferQueue_blocking(pSession)) ) {
    eCN eOperandClass = pOperand->eGetClassNo();

    if (eOperandClass == CN_MARKER) {
      Marker *pMarker = _pMarker(pOperand);
      eOM eOpMarker = pMarker->eOpMarker();

      AM::vDeleteOperand(pOperand);

      if (eOpMarker == OM_CANCELMARKER ||
          eOpMarker == OM_EXITMARKER)
        break;
    }
    else {
      AM::vDeleteOperand(pOperand);
    }
  }

}


/*----------------------------------------------------------------------------*/
/* process the result/output queue                                            */
/*----------------------------------------------------------------------------*/
void Output::vProcessBufferQueue(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  Operand* pOperand;
  
  while ( (pOperand = pPopBufferQueue_blocking(pSession)) ) {

    if (! bProcessOperand(pSession,pOperand))
      break;

  }
  
}

/*----------------------------------------------------------------------------*/
/* pop next operand from result queue                                         */
/*----------------------------------------------------------------------------*/
Operand* Output::pPopBufferQueue_blocking(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = _pOperand(pSession->m_pOutput->m_pOutputQueue->pPop());
    
    if (pOperand)
      return pOperand;
    
    AM::vNapUsec(1000,CN_OUTPUT);
  }
  
}

/*----------------------------------------------------------------------------*/
/* process an operand from the result/output queue                            */
/*----------------------------------------------------------------------------*/
bool Output::bProcessOperand(Session* pSession, Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUT,_FOOTER_);
  

  bool bContinue = true;
  
  //SP_DataBuffer spDataBuffer;
    
  eCN eOperandClass = pOperand->eGetClassNo();
  
  switch (eOperandClass) {
      
    // -- -- -- -- -- --
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      m_ixRowsProcessed += pCrwBuffer->nRowCount();
      vTrace("\n"_Stz);
      pCrwBuffer->vPrintRows(pSession->m_pStreamDef_result);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      m_ixRowsProcessed += pCsvBuffer->nRecords();
      vTrace("\n"_Stz);
      pCsvBuffer->vPrintRecords();
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
      m_ixRowsProcessed += pEntBuffer->nItems();
      vTrace("\n"_Stz);
      pEntBuffer->vPrintRefdRows();
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_INDEXDEF: {
      //vTrace("Pipeline Job: INDEXDEF\n"_Stz);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_INFOSET: {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      ASSERT( ! pInfoSet->szInfoSetNm()->bSame(C_NO_NAME) );
      m_ixRowsProcessed += pInfoSet->nInfoRows();
      vTrace("\n"_Stz);
      pInfoSet->vPrintRows(pSession->m_pStreamDef_result);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(pOperand);
      
      eOM eOpMarker = pMarker->eOpMarker();

      switch (eOpMarker) {
          
        case OM_CANCELMARKER: {
          vTrace("Query cancelled!!! (rows fetched so far %lld)\n"_Stz,
                 *m_ixRowsProcessed);
          AM::vDeleteOperand(pOperand);
          bContinue = false;
          goto L_EXIT;
          break;
        }
          
        case OM_DATAPULSE: {
          //vTrace("Pipeline Job: PULSEMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_DESTROYMARKER: {
          //vTrace("Pipeline Job: DESTROYMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_EOSMARKER: {
          //vTrace("Pipeline Job: EOSMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_ERRORMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
         
        case OM_ERROREDMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_EXITMARKER: {
          //vTrace("Pipeline Job: EXITMARKER\n"_Stz);
          //if ( ! bHasError )
          //  vTrace("success: %lld rows fetched\n"_Stz, *m_ixRowsProcessed);
          AM::vDeleteOperand(pOperand);
          pSession->m_bExitMarker = true;
          bContinue = false;
          goto L_EXIT;
          break;
        }
          
        case OM_PULSEDMARKER: {
          //vTrace("Pipeline Job: PULSEDMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_RETURNMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_RETURNEDMARKER: {
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        case OM_STARTMARKER: {
          //vTrace("Pipeline Job: STARTMARKER\n"_Stz);
          AM::vDeleteOperand(pOperand);
          break;
        }
          
        default:
          ASSERT(0);
      }
      break;
    }
      
    // -- -- -- -- -- --
      
    case CN_ROWBUFFER: {
      RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
//        if (AM::bDeveloper())
//          vTrace("[[RowBuffer %s ; Row=%lld]]\n"_Stz,
//                 spDataBuffer.sz(pRowBuffer),
//                 pRowBuffer->nRows());
      m_ixRowsProcessed += pRowBuffer->nRows();
      vTrace("\n"_Stz);
      pRowBuffer->vPrintRows();
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
            
    // -- -- -- -- -- --

    case CN_STREAMDEF: {
      //vTrace("Pipeline Job: STREAMMARKER\n"_Stz);
      if ( ! pSession->m_pStreamDef_result ) {
        pSession->m_pStreamDef_result = _pStreamDef(pOperand);
        
        // print the metadata
        if (AM::bMetadata()) {
          s4K sMetadata;
          pSession->m_pStreamDef_result->vToMetadata(sMetadata.szStz());
          vTrace("\n%s\n"_Stz, sMetadata.szStz());
        }
      }
      break;
    }
      
    // -- -- -- -- -- --
    case CN_STREAMSET: {
      StreamSet* pStreamSet = _pStreamSet(pOperand);
      ASSERT( ! pStreamSet->szInfoSetNm()->bSame(C_NO_NAME) );
      m_ixRowsProcessed += pStreamSet->nInfoRows();
      vTrace("\n"_Stz);
      pStreamSet->vPrintRows(pSession->m_pStreamDef_result);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    // -- -- -- -- -- --
      
    default:
      RASSERT(0);
      break;
  }
    
L_EXIT:
  return bContinue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
