#ifndef SENSENET_COMMONDEF_H_
#define SENSENET_COMMONDEF_H_


//BIAS和WEIGHT存放位置
#define CONV1_CONV_3x3_WEIGHT_DDR        0x81000000


#define CORES_WORKING   7

#define USER_HEAP_ADDR					 0x0081A000   //008103E0
#define USER_MSMC_ADDR         			 0x0C050001
#define USER_DDR_ADDR         			 0x8A000000
#define USER_DDR_ADDR1         			 0x8B000000

#define USER_DDR_ADDR2         			 0x8C000000  // ring ps

#define CALC_TOTAL_TIME					 1   //计算程序耗时

#define CODE_OPTIMIZATION                1   //代码级的优化
#define dspnum                           0   //dspnum

#define test_time                0   //是否开启计算x核程序耗时
#define CORENUM					 0   //计算x核程序耗时

#define ring                  1
#define ps                    0


//#define DSP0 	(0u)
//#define DSP1	(1u)
//#define DSP2 	(2u)
//#define DSP3	(3u)
//
////当前程序运行设备ID
//#define DeviceId  DSP0
#endif
