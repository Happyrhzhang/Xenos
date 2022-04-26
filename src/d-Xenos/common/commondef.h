#ifndef SENSENET_COMMONDEF_H_
#define SENSENET_COMMONDEF_H_


//BIAS��WEIGHT���λ��
#define CONV1_CONV_3x3_WEIGHT_DDR        0x81000000


#define CORES_WORKING   7

#define USER_HEAP_ADDR					 0x0081A000   //008103E0
#define USER_MSMC_ADDR         			 0x0C050001
#define USER_DDR_ADDR         			 0x8A000000
#define USER_DDR_ADDR1         			 0x8B000000

#define USER_DDR_ADDR2         			 0x8C000000  // ring ps

#define CALC_TOTAL_TIME					 1   //��������ʱ

#define CODE_OPTIMIZATION                1   //���뼶���Ż�
#define dspnum                           0   //dspnum

#define test_time                0   //�Ƿ�������x�˳����ʱ
#define CORENUM					 0   //����x�˳����ʱ

#define ring                  1
#define ps                    0


//#define DSP0 	(0u)
//#define DSP1	(1u)
//#define DSP2 	(2u)
//#define DSP3	(3u)
//
////��ǰ���������豸ID
//#define DeviceId  DSP0
#endif
