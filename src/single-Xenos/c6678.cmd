/****************************************************************************/
/*  C6678.cmd                                                               */
/*  Copyright (c) 2011 Texas Instruments Incorporated                       */
/*													                                     						*/
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an C6678              */
/*                 device.  Use it as a guideline.  You will want to        */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/
-c //automate initializate variable in run-time
-stack 0x18000
-heap  0x20000

/* Memory Map for C6678EVM */
MEMORY
{
	VECTORS:   			origin = 0x00800000  length = 0x00000200   /* 中断向量  *////////
	BOOT_CORE0: 	    origin = 0x00800200  length = 0x000000C0   ///////
    L2:   				origin = 0x008002C0  length = 0x00077D40   /* 512KB L2/SRAM *////////
    SHRAM:   			origin = 0x0C000000  length = 0x00050000   /* 4MB Multicore shared Memmory320KB给加载程序使用*/
    //SHRAM_LL2:          origin = 0x0C3C0000  length = 0x00030000
    Descriptor_DDR3:   	origin = 0x82000000  length = 0x06000000

    boot		: 	    o  = 0x70000000  l = 0x00000200
	FLASH       : 		o  = 0x70000200  l = 0x00400000
}

SECTIONS
{
/*
	.bootload : {} > boot
	.text     : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_TEXT_START),   RUN_START(RAM_TEXT_START),   SIZE(TEXT_SIZE)
	.cinit    : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_CINIT_START),  RUN_START(RAM_CINIT_START),  SIZE(CINIT_SIZE)
//	.pinit	  : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_PINIT_START),  RUN_START(RAM_PINIT_START),  SIZE(PINIT_SIZE)
	.const    : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_CONST_START),  RUN_START(RAM_CONST_START),  SIZE(CONST_SIZE)
	.switch   : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_SWITCH_START), RUN_START(RAM_SWITCH_START), SIZE(SWITCH_SIZE)
	.csl_vect : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_CSLVECT_START),   RUN_START(RAM_CSLVECT_START),   SIZE(CSLVECT_SIZE)
	.vecs     : {} > FLASH   RUN = VECTORS,  LOAD_START(FLASH_VECT_START),	 RUN_START(RAM_VECT_START),    SIZE(VECT_SIZE)
	.myboot{*.*<boot.obj>(.text)} : {} > FLASH   RUN = BOOT_CORE0, LOAD_START(FLASH_BOOT_START), RUN_START(RAM_BOOT_START), SIZE(BOOT_SIZE)
	.DDRText{MulticoreBoot.obj(.text)} : {} > FLASH   RUN = SHRAM, LOAD_START(FLASH_MULTICORE_START), RUN_START(RAM_MULTICORE_START), SIZE(MULTICORE_SIZE)
*/

	.text          >  SHRAM
	.cinit         >  SHRAM
	.const         >  SHRAM
	.switch        >  SHRAM
	.csl_vect      >  SHRAM
	.vecs          >  VECTORS

	.pinit         >  SHRAM
	.stack         >  L2                  /*local variable*/
	.bss           >  SHRAM
	.cio           >  SHRAM				  /*printf*/
	.data          >  SHRAM
	.sysmem        >  SHRAM
	.far           >  SHRAM
    .args          >  SHRAM
	.ppinfo        >  SHRAM
	.ppdata        >  SHRAM
	.core0_section >  SHRAM

  /* EABI sections */
    .binit         >  SHRAM
	.init_array    >  SHRAM
	.neardata      >  SHRAM
	.fardata       >  SHRAM
	.rodata        >  SHRAM
	.c6xabi.exidx  >  SHRAM
	.c6xabi.extab  >  SHRAM
	//platform_lib > Descriptor_DDR3

	//QMSS_Data.linkingRAM1 		> 	SHRAM
	//QMSS_Data.Descriptor_SL2 	> 	SHRAM
	//PacketData.buffer_SL2 		> 	SHRAM
	//QMSS_Data.Descriptor_LL2 	> 	SHRAM     //SHRAM
	//PacketData.buffer_LL2 		> 	SHRAM_LL2 //SHRAM
	//QMSS_Data.Descriptor_DDR 	> 	Descriptor_DDR3
	//PacketData.buffer_DDR 		> 	Descriptor_DDR3
}
