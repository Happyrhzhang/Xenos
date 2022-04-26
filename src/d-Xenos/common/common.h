#ifndef SENSENET_COMMON_H_
#define SENSENET_COMMON_H_


#include "commondef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <c6x.h>
#include <ti/csl/csl.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_sem.h>
#include <ti/csl/csl_semAux.h>

struct net_io
{
	float* input_image1;
//	float* output_img1;
//	float* output_img2;
//	float* output_img3;
//	float* output_img4;
//	float* output_img5;
//	float* output_img6;
//	float* output_img7;
//	float* output_img8;
//	float* output_img9;
//	float* output_img10;
//	float* output_img11;
	float* output_img12;
	float* output_img13;
	float* output_img14;
	float* output_img15;
	float* output_img16;
	float* output_img17;
	float* output_img18;
	float* output_img19;
	float* output_img20;
	float* output_img21;
	float* output_img22;
	float* output_img23;
	float* output_img24;
	float* output_img25;
	float* output_img26;
	float* output_img27;
	float* output_img28;
	float* output_img29;
	float* output_img30;

	float* tiling_input;
	float* tiling_output;
	//ring2/ps6
	float* tiling_buffer_write[6];
	float* tiling_buffer_read[6];

	float* tobig_ddr_mem[4];
};

struct weight_DDR
{
	float* weight1_DDR;
	float* weight2_DDR;
	float* weight3_DDR;
	float* weight4_DDR;
	float* weight5_DDR;
	float* weight6_DDR;
	float* weight7_DDR;
	float* weight8_DDR;
	float* weight9_DDR;
	float* weight10_DDR;
	float* weight11_DDR;
	float* weight12_DDR;
	float* weight13_DDR;
	float* weight14_DDR;
	float* weight15_DDR;
	float* weight16_DDR;
	float* weight17_DDR;
	float* weight18_DDR;
	float* weight19_DDR;
	float* weight20_DDR;
	float* weight21_DDR;
	float* weight22_DDR;
	float* weight23_DDR;
	float* weight24_DDR;
	float* weight25_DDR;
	float* weight26_DDR;
	float* weight27_DDR;
	float* weight28_DDR;
};

struct bias_DDR
{
	float* bias1_DDR;
	float* bias2_DDR;
	float* bias3_DDR;
	float* bias4_DDR;
	float* bias5_DDR;
	float* bias6_DDR;
	float* bias7_DDR;
	float* bias8_DDR;
	float* bias9_DDR;
	float* bias10_DDR;
	float* bias11_DDR;
	float* bias12_DDR;
	float* bias13_DDR;
	float* bias14_DDR;
	float* bias15_DDR;
	float* bias16_DDR;
	float* bias17_DDR;
	float* bias18_DDR;
	float* bias19_DDR;
	float* bias20_DDR;
	float* bias21_DDR;
	float* bias22_DDR;
	float* bias23_DDR;
	float* bias24_DDR;
	float* bias25_DDR;
	float* bias26_DDR;
	float* bias27_DDR;
	float* bias28_DDR;
};

struct weight
{
	float* weight1;
	float* weight2;
	float* weight3;
	float* weight4;
	float* weight5;
	float* weight6;
	float* weight7;
	float* weight8;
	float* weight9;
	float* weight10;
	float* weight11;
	float* weight12;
	float* weight13;
	float* weight14;
	float* weight15;
	float* weight16;
	float* weight17;
	float* weight18;
	float* weight19;
	float* weight20;
	float* weight21;
	float* weight22;
	float* weight23;
	float* weight24;
	float* weight25;
	float* weight26;
	float* weight27;
	float* weight28;
};

struct bias
{
	float* bias1;
	float* bias2;
	float* bias3;
	float* bias4;
	float* bias5;
	float* bias6;
	float* bias7;
	float* bias8;
	float* bias9;
	float* bias10;
	float* bias11;
	float* bias12;
	float* bias13;
	float* bias14;
	float* bias15;
	float* bias16;
	float* bias17;
	float* bias18;
	float* bias19;
	float* bias20;
	float* bias21;
	float* bias22;
	float* bias23;
	float* bias24;
	float* bias25;
	float* bias26;
	float* bias27;
	float* bias28;
};

struct weight_offset
{
	int weight1_offset;
	int weight2_offset;
	int weight3_offset;
	int weight4_offset;
	int weight5_offset;
	int weight6_offset;
	int weight7_offset;
	int weight8_offset;
	int weight9_offset;
	int weight10_offset;
	int weight11_offset;
	int weight12_offset;
	int weight13_offset;
	int weight14_offset;
	int weight15_offset;
	int weight16_offset;
	int weight17_offset;
	int weight18_offset;
	int weight19_offset;
	int weight20_offset;
	int weight21_offset;
	int weight22_offset;
	int weight23_offset;
	int weight24_offset;
	int weight25_offset;
	int weight26_offset;
	int weight27_offset;
	int weight28_offset;
};

struct bias_offset
{
	int bias1_offset;
	int bias2_offset;
	int bias3_offset;
	int bias4_offset;
	int bias5_offset;
	int bias6_offset;
	int bias7_offset;
	int bias8_offset;
	int bias9_offset;
	int bias10_offset;
	int bias11_offset;
	int bias12_offset;
	int bias13_offset;
	int bias14_offset;
	int bias15_offset;
	int bias16_offset;
	int bias17_offset;
	int bias18_offset;
	int bias19_offset;
	int bias20_offset;
	int bias21_offset;
	int bias22_offset;
	int bias23_offset;
	int bias24_offset;
	int bias25_offset;
	int bias26_offset;
	int bias27_offset;
	int bias28_offset;
};

struct other_offset
{

};
struct math_cmp
{

};


struct sensenet_t
{
	unsigned int ui_core;

	char* image_raw;
	float* image;
	//float* tiling_buffer_sram[8][2];    //共享上缓存相邻行数据

	struct net_io* io;

	struct weight* weight;
	struct bias* bias;

	struct weight_DDR* weight_ddr;
	struct bias_DDR* bias_ddr;

	struct weight_offset* weight_offset;
	struct bias_offset* bias_offset;
	struct other_offset* other_offset;
	struct math_cmp* math_cmp;

	volatile char* p_corestart;   //表示1-7核可以开始工作
	volatile char* p_coreready;   //表示1-7核准备完成
};

typedef struct sensenet_t sensenet_t;

/****************************************************************************
 * Memory functions
 ****************************************************************************/

void *snet_malloc( int i_size, unsigned int* startAddress);
void  snet_free( void *);

/****************************************************************************
 * Cores Communication functions
 ****************************************************************************/

void SetCoreReadyFlag(sensenet_t* s);
void WaitAllCoreReady(sensenet_t* s);
void WaitCoreStartFlag(sensenet_t* s);
void SetAllCoreStart(sensenet_t* s);
bool IsAllCoreDone(sensenet_t* s);
void ResetCoreStartFlag(sensenet_t* s);

#endif
