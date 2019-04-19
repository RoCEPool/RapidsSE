/*============================================================================*/
/* Prompt_tpch.cpp -- show tpch                                               */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*   gpAttachMap;

PromptNote mPromptNote_tpch (
  PF_TPCH,           // prompt function
  "Prompt_tpch"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "tpch"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_tpch,      // prompt function
  true,              // enabled

  true,              // visible
  "tpch"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tpch(SePrompt* pPrompt)
{
  printf("C_1_BYTE  %llu\n",C_1_BYTE);
  printf("C_2_BYTES %llu\n",C_2_BYTES);
  printf("C_3_BYTES %llu\n",C_3_BYTES);
  printf("C_4_BYTES %llu\n",C_4_BYTES);
  printf("C_5_BYTES %llu\n",C_5_BYTES);
  printf("C_6_BYTES %llu\n",C_6_BYTES);
  printf("C_7_BYTES %llu\n",C_7_BYTES);
  printf("C_8_BYTES %llu\n",C_8_BYTES);
  
  const i8 C_TPCH_CUSTOMER =  150 * C_1K;
  const i8 C_TPCH_LINEITEM = 6000 * C_1K;
  const i8 C_TPCH_ORDERS   = 1500 * C_1K;
  const i8 C_TPCH_PART     =  200 * C_1K;
  const i8 C_TPCH_PARTSUPP =  800 * C_1K;
  const i8 C_TPCH_SUPPLIER =   10 * C_1K;

  const i8 C_TPCH_NATION   = 25;
  const i8 C_TPCH_REGION   = 5;

  s32 sMagSF;
  
  s32 sMagCustomer;
  s32 sMagLineItem;
  s32 sMagOrders;
  s32 sMagPart;
  s32 sMagPartSupp;
  s32 sMagSupplier;

  s32 sByteMagCustomer;
  s32 sByteMagLineItem;
  s32 sByteMagOrders;
  s32 sByteMagPart;
  s32 sByteMagPartSupp;
  s32 sByteMagSupplier;

  vTrace("        Customer    LineItem    Orders      Part        PartSupp     Supplier\n"_Stz);
  
  for (i8 iSF = 1; iSF <= C_1M; iSF *= 10) {
    Magnitudes::vUnitMag(iSF,sMagSF);
    vTrace("SF%-4s: "_Stz,sMagSF.aChar());
    
    Magnitudes::vUnitMag(C_TPCH_CUSTOMER * iSF,sMagCustomer);
    Magnitudes::vUnitMag(C_TPCH_LINEITEM * iSF,sMagLineItem);
    Magnitudes::vUnitMag(C_TPCH_ORDERS   * iSF,sMagOrders);
    Magnitudes::vUnitMag(C_TPCH_PART     * iSF,sMagPart);
    Magnitudes::vUnitMag(C_TPCH_PARTSUPP * iSF,sMagPartSupp);
    Magnitudes::vUnitMag(C_TPCH_SUPPLIER * iSF,sMagSupplier);

    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_CUSTOMER * iSF),sByteMagCustomer);
    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_LINEITEM * iSF),sByteMagLineItem);
    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_ORDERS   * iSF),sByteMagOrders);
    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_PART     * iSF),sByteMagPart);
    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_PARTSUPP * iSF),sByteMagPartSupp);
    Magnitudes::vByteMag(IntLib::iByteSpace(1,C_TPCH_SUPPLIER * iSF),sByteMagSupplier);

    vTrace("%4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s)\n"_Stz,
           sMagCustomer.aChar(),sByteMagCustomer.aChar(),
           sMagLineItem.aChar(),sByteMagLineItem.aChar(),
           sMagOrders.aChar(),  sByteMagOrders.aChar(),
           sMagPart.aChar(),    sByteMagPart.aChar(),
           sMagPartSupp.aChar(),sByteMagPartSupp.aChar(),
           sMagSupplier.aChar(),sByteMagSupplier.aChar());
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
