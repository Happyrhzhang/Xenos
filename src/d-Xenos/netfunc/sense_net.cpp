#include "sense_net.h"
/****************************************************************
*   作者：BUAA
*   版本：V4.0
*   创建时间：2021年-2-5 19:00
*   描述说明：已完成第1层到第3层分块动态重构；1x1卷积和pooling层融合；第3层到第7层输出通道并行。
*   修改历史：
*
*****************************************************************/

//DSP上所有空间分配
//	unsigned int g_startHeapAddress = USER_HEAP_ADDR;l2 512KB
//	unsigned int g_startMSMCAddress = USER_MSMC_ADDR;共享 4M
//	unsigned int g_startDDR3Address = USER_DDR_ADDR;ddr ~
sensenet_t* sensenet_open(unsigned int core)
{
	int i;
	unsigned int reuse_msmc_swap0, reuse_msmc_swap1, reuse_msmc_swap2,reuse_msmc_swap3,reuse_msmc_swap4,reuse_msmc_swap5;

	unsigned int g_startHeapAddress = USER_HEAP_ADDR;
	unsigned int g_startMSMCAddress = USER_MSMC_ADDR;
	unsigned int g_startDDR3Address = USER_DDR_ADDR;
	unsigned int g_startDDR3Address1 = USER_DDR_ADDR1;
	unsigned int g_startDDR3Address2 = USER_DDR_ADDR2;
	struct net_io* io = (struct net_io*)snet_malloc(sizeof(struct net_io), &g_startHeapAddress);
	struct weight* weight = (struct weight*)snet_malloc(sizeof(struct weight), &g_startHeapAddress);
	struct bias* bias = (struct bias*)snet_malloc(sizeof(struct bias), &g_startHeapAddress);
	struct weight_DDR* weight_ddr = (struct weight_DDR*)snet_malloc(sizeof(struct weight_DDR), &g_startHeapAddress);
	struct bias_DDR* bias_ddr = (struct bias_DDR*)snet_malloc(sizeof(struct bias_DDR), &g_startHeapAddress);
	struct weight_offset* weight_offset = (struct weight_offset*)snet_malloc(sizeof(struct weight_offset), &g_startHeapAddress);
	struct bias_offset* bias_offset = (struct bias_offset*)snet_malloc(sizeof(struct bias_offset), &g_startHeapAddress);
	struct other_offset* other_offset = (struct other_offset*)snet_malloc(sizeof(struct other_offset), &g_startHeapAddress);
	struct math_cmp* math_cmp = (struct math_cmp*)snet_malloc(sizeof(struct math_cmp), &g_startHeapAddress);
	memset(io, 0, sizeof(struct net_io));
	memset(weight, 0, sizeof(struct weight));
	memset(bias, 0, sizeof(struct bias));
	memset(weight_ddr, 0, sizeof(struct weight_DDR));
	memset(bias_ddr, 0, sizeof(struct bias_DDR));
	memset(weight_offset, 0, sizeof(struct weight_offset));
	memset(bias_offset, 0, sizeof(struct bias_offset));
	memset(other_offset, 0, sizeof(struct other_offset));
	memset(math_cmp, 0, sizeof(struct math_cmp));

	sensenet_t* s = (sensenet_t*)snet_malloc(sizeof(sensenet_t), &g_startHeapAddress);
	memset(s, 0, sizeof(sensenet_t));
	s->io = io;
	s->weight = weight;
	s->bias = bias;
	s->weight_ddr = weight_ddr;
	s->bias_ddr = bias_ddr;
	s->weight_offset = weight_offset;
	s->bias_offset = bias_offset;
	s->other_offset = other_offset;
	s->math_cmp = math_cmp;

	unsigned int r1_startHeapAddress = g_startHeapAddress;
	unsigned int r2_startHeapAddress = g_startHeapAddress;
	unsigned int r3_startHeapAddress = g_startHeapAddress;
	unsigned int r4_startHeapAddress = g_startHeapAddress;
	unsigned int r5_startHeapAddress = g_startHeapAddress;
	unsigned int r6_startHeapAddress = g_startHeapAddress;
	unsigned int r7_startHeapAddress = g_startHeapAddress;
	unsigned int r8_startHeapAddress = g_startHeapAddress;


	s->ui_core = core;

	s->io->input_image1 = (float*)snet_malloc((img_height[0] + 2) * (img_width[0] + 2) * input_channel[0] * sizeof(float), &g_startMSMCAddress);

	s->weight_ddr->weight1_DDR = (float *)CONV1_CONV_3x3_WEIGHT_DDR;
	s->bias_ddr->bias1_DDR = s->weight_ddr->weight1_DDR + 3*3*input_channel[0]*output_channel[0];

	s->weight_ddr->weight2_DDR = s->bias_ddr->bias1_DDR + output_channel[0];
	s->bias_ddr->bias2_DDR = s->weight_ddr->weight2_DDR + 3*3*input_channel[1];
	s->weight_ddr->weight3_DDR = s->bias_ddr->bias2_DDR + output_channel[1];
	s->bias_ddr->bias3_DDR = s->weight_ddr->weight3_DDR + input_channel[2] * output_channel[2];

	s->weight_ddr->weight4_DDR = s->bias_ddr->bias3_DDR + output_channel[2];
	s->bias_ddr->bias4_DDR = s->weight_ddr->weight4_DDR + 3*3*input_channel[3];
	s->weight_ddr->weight5_DDR = s->bias_ddr->bias4_DDR + output_channel[3];
	s->bias_ddr->bias5_DDR = s->weight_ddr->weight5_DDR + input_channel[4] * output_channel[4];

	s->weight_ddr->weight6_DDR = s->bias_ddr->bias5_DDR + output_channel[4];
	s->bias_ddr->bias6_DDR = s->weight_ddr->weight6_DDR + 3*3*input_channel[5];
	s->weight_ddr->weight7_DDR = s->bias_ddr->bias6_DDR + output_channel[5];
	s->bias_ddr->bias7_DDR = s->weight_ddr->weight7_DDR + input_channel[6] * output_channel[6];

	s->weight_ddr->weight8_DDR = s->bias_ddr->bias7_DDR + output_channel[6];
	s->bias_ddr->bias8_DDR = s->weight_ddr->weight8_DDR + 3*3*input_channel[7];
	s->weight_ddr->weight9_DDR = s->bias_ddr->bias8_DDR + output_channel[7];
	s->bias_ddr->bias9_DDR = s->weight_ddr->weight9_DDR + input_channel[8] * output_channel[8];

	s->weight_ddr->weight10_DDR = s->bias_ddr->bias9_DDR + output_channel[8];
	s->bias_ddr->bias10_DDR = s->weight_ddr->weight10_DDR + 3*3*input_channel[9];
	s->weight_ddr->weight11_DDR = s->bias_ddr->bias10_DDR + output_channel[9];
	s->bias_ddr->bias11_DDR = s->weight_ddr->weight11_DDR + input_channel[10] * output_channel[10];

	s->weight_ddr->weight12_DDR = s->bias_ddr->bias11_DDR + output_channel[10];
	s->bias_ddr->bias12_DDR = s->weight_ddr->weight12_DDR + 3*3*input_channel[11];
	s->weight_ddr->weight13_DDR = s->bias_ddr->bias12_DDR + output_channel[11];
	s->bias_ddr->bias13_DDR = s->weight_ddr->weight13_DDR + input_channel[12] * output_channel[12];
	//
	s->weight_ddr->weight14_DDR = s->bias_ddr->bias13_DDR + output_channel[12];
	s->bias_ddr->bias14_DDR = s->weight_ddr->weight14_DDR + 3*3*input_channel[13];
	s->weight_ddr->weight15_DDR = s->bias_ddr->bias14_DDR + output_channel[13];
	s->bias_ddr->bias15_DDR = s->weight_ddr->weight15_DDR + input_channel[13] * output_channel[13];

	s->weight_ddr->weight16_DDR = s->bias_ddr->bias15_DDR + output_channel[13];
	s->bias_ddr->bias16_DDR = s->weight_ddr->weight16_DDR + 3*3*input_channel[13];
	s->weight_ddr->weight17_DDR = s->bias_ddr->bias16_DDR + output_channel[13];
	s->bias_ddr->bias17_DDR = s->weight_ddr->weight17_DDR + input_channel[13] * output_channel[13];

	s->weight_ddr->weight18_DDR = s->bias_ddr->bias17_DDR + output_channel[13];
	s->bias_ddr->bias18_DDR = s->weight_ddr->weight18_DDR + 3*3*input_channel[13];
	s->weight_ddr->weight19_DDR = s->bias_ddr->bias18_DDR + output_channel[13];
	s->bias_ddr->bias19_DDR = s->weight_ddr->weight19_DDR + input_channel[13] * output_channel[13];

	s->weight_ddr->weight20_DDR = s->bias_ddr->bias19_DDR + output_channel[13];
	s->bias_ddr->bias20_DDR = s->weight_ddr->weight20_DDR + 3*3*input_channel[13];
	s->weight_ddr->weight21_DDR = s->bias_ddr->bias20_DDR + output_channel[13];
	s->bias_ddr->bias21_DDR = s->weight_ddr->weight21_DDR + input_channel[13] * output_channel[13];

	s->weight_ddr->weight22_DDR = s->bias_ddr->bias21_DDR + output_channel[13];
	s->bias_ddr->bias22_DDR = s->weight_ddr->weight22_DDR + 3*3*input_channel[13];
	s->weight_ddr->weight23_DDR = s->bias_ddr->bias22_DDR + output_channel[13];
	s->bias_ddr->bias23_DDR = s->weight_ddr->weight23_DDR + input_channel[13] * output_channel[13];
	//
	s->weight_ddr->weight24_DDR = s->bias_ddr->bias23_DDR + output_channel[13];
	s->bias_ddr->bias24_DDR = s->weight_ddr->weight24_DDR + 3*3*input_channel[14];
	s->weight_ddr->weight25_DDR = s->bias_ddr->bias24_DDR + output_channel[14];
	s->bias_ddr->bias25_DDR = s->weight_ddr->weight25_DDR + input_channel[15] * output_channel[15];

	s->weight_ddr->weight26_DDR = s->bias_ddr->bias25_DDR + output_channel[15];
	s->bias_ddr->bias26_DDR = s->weight_ddr->weight26_DDR + 3*3*input_channel[16];
	s->weight_ddr->weight27_DDR = s->bias_ddr->bias26_DDR + output_channel[16];
	s->bias_ddr->bias27_DDR = s->weight_ddr->weight27_DDR + input_channel[17] * output_channel[17];

	s->weight_ddr->weight27_DDR = s->bias_ddr->bias26_DDR + output_channel[16];
	s->bias_ddr->bias27_DDR = s->weight_ddr->weight27_DDR + input_channel[17] * output_channel[17];

	s->weight_ddr->weight28_DDR = s->bias_ddr->bias27_DDR + output_channel[17];
	s->bias_ddr->bias28_DDR = s->weight_ddr->weight28_DDR + input_channel[18] * output_channel[18];

	/***********************************************************************************************************/
	s->p_corestart = (char*)snet_malloc(sizeof(char) * CORES_WORKING, &g_startDDR3Address);
	s->p_coreready = (char*)snet_malloc(sizeof(char) * CORES_WORKING, &g_startDDR3Address);

	if (s->ui_core == 0)
	{
		for (i = 0; i < CORES_WORKING; i++)
		{
			*(s->p_corestart + i) = 0;
		}
		for (i = 0; i < CORES_WORKING; i++)
		{
			*(s->p_coreready + i) = 0;
		}

		CACHE_wbInvL1d((void*)(s->p_corestart), sizeof(char) * CORES_WORKING, CACHE_WAIT);
		CACHE_wbInvL1d((void*)(s->p_coreready), sizeof(char) * CORES_WORKING, CACHE_WAIT);
	}

	reuse_msmc_swap0 = g_startMSMCAddress;
	reuse_msmc_swap1 = g_startMSMCAddress;
	reuse_msmc_swap2 = g_startMSMCAddress;
	reuse_msmc_swap3 = g_startMSMCAddress;
	reuse_msmc_swap4 = g_startMSMCAddress;
	reuse_msmc_swap5 = g_startMSMCAddress;

	s->io->tiling_input = (float*)snet_malloc(img_tiling[3][0][0] * img_tiling[3][0][1] * output_channel[2] * sizeof(float), &reuse_msmc_swap4);
	s->io->tiling_output = (float*)snet_malloc(img_tiling[3][0][0] * img_tiling[3][0][1] * output_channel[2] * sizeof(float), &reuse_msmc_swap4);

//	s->io->output_img1 = (float*)snet_malloc(output_channel[0]*img_height[1]*img_width[1] * sizeof(float), &reuse_msmc_swap0);     //共享112x112x32 1.53M
//	s->io->output_img2 = (float*)snet_malloc(output_channel[1]*img_height[2]*img_width[2] * sizeof(float), &reuse_msmc_swap0);     //共享 112x112x32 1.53M
//	s->io->output_img3 = (float*)snet_malloc(output_channel[2]*img_height[3]*img_width[3] * sizeof(float), &g_startDDR3Address1);     //共享 112x112x64 3.0625M   ////////////////////youhua
//	s->io->output_img4 = (float*)snet_malloc(output_channel[3]*img_height[4]*img_width[4] * sizeof(float), &g_startDDR3Address1);     //共享 56x56x64 784KB    /////////////////   youhua
//	s->io->output_img5 = (float*)snet_malloc(output_channel[4]*img_height[5]*img_width[5] * sizeof(float), &reuse_msmc_swap1);     //共享 56x56x128 1.53M
//	s->io->output_img6 = (float*)snet_malloc(output_channel[5]*img_height[6]*img_width[6] * sizeof(float), &reuse_msmc_swap1);     //共享 56x56x128 1.53M
//	s->io->output_img7 = (float*)snet_malloc(output_channel[6]*img_height[7]*img_width[7] * sizeof(float), &reuse_msmc_swap2);       //共享56x56x128 1.53M
//	s->io->output_img8 = (float*)snet_malloc(output_channel[7]*img_height[8]*img_width[8] * sizeof(float), &reuse_msmc_swap2);     //共享28x28x128 392K
//	s->io->output_img9 = (float*)snet_malloc(output_channel[8]*img_height[9]*img_width[9] * sizeof(float), &reuse_msmc_swap2);     //共享 28x28x256 784K
//	s->io->output_img10 = (float*)snet_malloc(output_channel[9]*img_height[10]*img_width[10] * sizeof(float), &reuse_msmc_swap2);    //共享28x28x256 784K
//	s->io->output_img11 = (float*)snet_malloc(output_channel[10]*img_height[11]*img_width[11] * sizeof(float), &reuse_msmc_swap3);   //共享 28x28x256 784K
	s->io->output_img12 = (float*)snet_malloc(output_channel[11]*img_height[12]*img_width[12] * sizeof(float), &reuse_msmc_swap0); //共享 14x14x256  196k
	s->io->output_img13 = (float*)snet_malloc(output_channel[12]*img_height[13]*img_width[13] * sizeof(float), &reuse_msmc_swap0); //共享 14x14x512 392k

	s->io->output_img14 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享 14x14x512 392k
	s->io->output_img15 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享  14x14x512 392k
	s->io->output_img16 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享  14x14x512 392k
	s->io->output_img17 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享  14x14x512 392k
	s->io->output_img18 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享  14x14x512 392k
	s->io->output_img19 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap1);   //共享  14x14x512 392k
	s->io->output_img20 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap2);   //共享  14x14x512 392k
	s->io->output_img21 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap2);   //共享  14x14x512 392k
	s->io->output_img22 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap2);   //共享  14x14x512 392k
	s->io->output_img23 = (float*)snet_malloc(output_channel[13]*img_height[14]*img_width[14] * sizeof(float), &reuse_msmc_swap2);   //共享  14x14x512 392k

	s->io->output_img24 = (float*)snet_malloc(output_channel[14]*img_height[15]*img_width[15] * sizeof(float), &reuse_msmc_swap3);   //共享 7x7x512  98k
	s->io->output_img25 = (float*)snet_malloc(output_channel[15]*img_height[16]*img_width[16] * sizeof(float), &reuse_msmc_swap3);   //共享7x7x1024 196k
	s->io->output_img26 = (float*)snet_malloc(output_channel[16]*img_height[17]*img_width[17] * sizeof(float), &reuse_msmc_swap3);   //共享7x7x1024 196k
	s->io->output_img27 = (float*)snet_malloc(output_channel[17]*img_height[18]*img_width[18] * sizeof(float), &reuse_msmc_swap3);   //共享7x7x1024 196k/2

	s->io->output_img28 = (float*)snet_malloc(output_channel[18]*img_height[19]*img_width[19] * sizeof(float), &reuse_msmc_swap3);   //共享7x7x1024 196k/2
	//concat
	s->io->output_img29 = (float*)snet_malloc(output_channel[19]*img_height[19]*img_width[19] * sizeof(float), &reuse_msmc_swap3);   //共享7x7x1024 196k

	s->io->output_img30 = (float*)snet_malloc(output_channel[19] * sizeof(float), &reuse_msmc_swap3);   //共享 4kb

	for (i = 0; i < 4; i++)
	{
		s->io->tobig_ddr_mem[i] =  (float*)snet_malloc(output_channel[11]*img_height[12]*img_width[12] * sizeof(float) / 4, &reuse_msmc_swap4); //共享 14x14x256  196k
	}

#if ring
	for (i = 0; i < 2; i++)
	{
	    s->io->tiling_buffer_write[i] = (float*)snet_malloc(img_tiling[3][0][0]  * output_channel[2] * sizeof(float), &reuse_msmc_swap4);   //252KB for each core
	}
	for (i = 0; i < 2; i++)
	{
		s->io->tiling_buffer_read[i] = (float*)snet_malloc(img_tiling[3][0][0]  * output_channel[2] * sizeof(float), &reuse_msmc_swap4);   //252KB for each core
	}
#endif

#if ps
	for (i = 0; i < 6; i++)
	{
		s->io->tiling_buffer_write[i] = (float*)snet_malloc(img_tiling[3][0][0]  * output_channel[2] * sizeof(float), &reuse_msmc_swap4);    //ddr maxsize
	}
	for (i = 0; i < 6; i++)
	{
		s->io->tiling_buffer_read[i] = (float*)snet_malloc(img_tiling[3][0][0]  * output_channel[2] * sizeof(float), &reuse_msmc_swap4);    //ddr maxsize
	}
#endif

	{
		/***********************************L2空间复用,最后申请***********************************/
		s->weight->weight1 = (float *)snet_malloc(3*3*input_channel[0]*channel_group[0][core][0]* sizeof(float), &r1_startHeapAddress); //3 3 3 32 0.42K
		s->bias->bias1 = (float *)snet_malloc(channel_group[0][core][0] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight2 = (float *)snet_malloc(3*3*channel_group[1][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*32 0.14K
		s->bias->bias2 = (float *)snet_malloc(channel_group[1][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight3 = (float *)snet_malloc(input_channel[2]*channel_group[2][core][2]* sizeof(float), &r1_startHeapAddress);//32*64 1K
		s->bias->bias3 = (float *)snet_malloc(channel_group[2][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight4 = (float *)snet_malloc(3*3*channel_group[3][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*64 0.28K
		s->bias->bias4 = (float *)snet_malloc(channel_group[3][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight5 = (float *)snet_malloc(input_channel[4]*channel_group[4][core][2]* sizeof(float), &r1_startHeapAddress);//64*128 4K
		s->bias->bias5 = (float *)snet_malloc(channel_group[4][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight6 = (float *)snet_malloc(3*3*channel_group[5][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*128 0.56K
		s->bias->bias6 = (float *)snet_malloc(channel_group[5][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight7 = (float *)snet_malloc(input_channel[6]*channel_group[6][core][2]* sizeof(float), &r1_startHeapAddress);//128*128 8K
		s->bias->bias7 = (float *)snet_malloc(channel_group[6][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight8 = (float *)snet_malloc(3*3*channel_group[7][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*128 0.56K
		s->bias->bias8 = (float *)snet_malloc(channel_group[7][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight9 = (float *)snet_malloc(input_channel[8]*channel_group[8][core][2]* sizeof(float), &r1_startHeapAddress);//256*128 16K
		s->bias->bias9 = (float *)snet_malloc(channel_group[8][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight10 = (float *)snet_malloc(3*3*channel_group[9][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*256 1.12K
		s->bias->bias10 = (float *)snet_malloc(channel_group[9][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight11 = (float *)snet_malloc(input_channel[10]*channel_group[10][core][2]* sizeof(float), &r1_startHeapAddress);//256*256 32K
		s->bias->bias11 = (float *)snet_malloc(channel_group[10][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight12 = (float *)snet_malloc(3*3*channel_group[11][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*256 1.12K
		s->bias->bias12 = (float *)snet_malloc(channel_group[11][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight13 = (float *)snet_malloc(input_channel[12]*channel_group[12][core][2]* sizeof(float), &r1_startHeapAddress);//256*512 64K
		s->bias->bias13 = (float *)snet_malloc(channel_group[12][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight14 = (float *)snet_malloc(3*3*channel_group[13][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*512 2.25K
		s->bias->bias14 = (float *)snet_malloc(channel_group[13][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight15 = (float *)snet_malloc(input_channel[13]*channel_group[13][core][2]* sizeof(float), &r1_startHeapAddress);//512*512 128K
		s->bias->bias15 = (float *)snet_malloc(channel_group[13][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight16 = (float *)snet_malloc(3*3*channel_group[13][core][0]* sizeof(float), &r1_startHeapAddress);//3*3*512 2.25K
		s->bias->bias16 = (float *)snet_malloc(channel_group[13][core][0] * sizeof(float), &r1_startHeapAddress);
		s->weight->weight17 = (float *)snet_malloc(input_channel[13]*channel_group[13][core][2]* sizeof(float), &r1_startHeapAddress);//512*512 128K
		s->bias->bias17 = (float *)snet_malloc(channel_group[13][core][2] * sizeof(float), &r1_startHeapAddress);

		s->weight->weight18 = (float *)snet_malloc(3*3*channel_group[13][core][0]* sizeof(float), &r2_startHeapAddress);//3*3*512 2.25K
		s->bias->bias18 = (float *)snet_malloc(channel_group[13][core][0] * sizeof(float), &r2_startHeapAddress);
		s->weight->weight19 = (float *)snet_malloc(input_channel[13]*channel_group[13][core][2]* sizeof(float), &r2_startHeapAddress);//512*512 128K
		s->bias->bias19 = (float *)snet_malloc(channel_group[13][core][2] * sizeof(float), &r2_startHeapAddress);

		s->weight->weight20 = (float *)snet_malloc(3*3*channel_group[13][core][0]* sizeof(float), &r2_startHeapAddress);//3*3*512 2.25K
		s->bias->bias20 = (float *)snet_malloc(channel_group[13][core][0] * sizeof(float), &r2_startHeapAddress);
		s->weight->weight21 = (float *)snet_malloc(input_channel[13]*channel_group[13][core][2]* sizeof(float), &r2_startHeapAddress);//512*512 128K
		s->bias->bias21 = (float *)snet_malloc(channel_group[13][core][2] * sizeof(float), &r2_startHeapAddress);

		s->weight->weight22 = (float *)snet_malloc(3*3*channel_group[13][core][0]* sizeof(float), &r2_startHeapAddress);//3*3*512 2.25K
		s->bias->bias22 = (float *)snet_malloc(channel_group[13][core][0] * sizeof(float), &r2_startHeapAddress);
		s->weight->weight23 = (float *)snet_malloc(input_channel[13]*channel_group[13][core][2]* sizeof(float), &r2_startHeapAddress);//512*512 128K
		s->bias->bias23 = (float *)snet_malloc(channel_group[13][core][2] * sizeof(float), &r2_startHeapAddress);

		s->weight->weight24 = (float *)snet_malloc(3*3*channel_group[14][core][0]* sizeof(float), &r3_startHeapAddress);//3*3*512 2.25K
		s->bias->bias24 = (float *)snet_malloc(channel_group[14][core][0] * sizeof(float), &r3_startHeapAddress);
		s->weight->weight25 = (float *)snet_malloc(input_channel[15]*channel_group[15][core][2]* sizeof(float), &r3_startHeapAddress);//512*1024 256K
		s->bias->bias25 = (float *)snet_malloc(channel_group[15][core][2] * sizeof(float), &r3_startHeapAddress);

		s->weight->weight26 = (float *)snet_malloc(3*3*channel_group[16][core][0]* sizeof(float), &r3_startHeapAddress);//3*3*1024 4.5K
		s->bias->bias26 = (float *)snet_malloc(channel_group[16][core][0] * sizeof(float), &r3_startHeapAddress);
		s->weight->weight27 = (float *)snet_malloc(input_channel[17]*channel_group[17][core][2]* sizeof(float), &r4_startHeapAddress);//1024*512 256K
		s->bias->bias27 = (float *)snet_malloc(channel_group[17][core][2] * sizeof(float), &r4_startHeapAddress);

		s->weight->weight28 = (float *)snet_malloc(input_channel[18]*channel_group[18][core][2]* sizeof(float), &r5_startHeapAddress);//1024*512 256K
		s->bias->bias28 = (float *)snet_malloc(channel_group[18][core][2] * sizeof(float), &r5_startHeapAddress);

		for (i = 0; i < core; i++)
		{
			s->weight_offset->weight1_offset += 3*3*input_channel[0]*channel_group[0][i][0];
			s->bias_offset->bias1_offset += channel_group[0][i][0];

			s->weight_offset->weight2_offset += 3*3*channel_group[1][i][0];
			s->bias_offset->bias2_offset += channel_group[1][i][0];
			s->weight_offset->weight3_offset += input_channel[2]*channel_group[2][i][2];
			s->bias_offset->bias3_offset += channel_group[2][i][2];

			s->weight_offset->weight4_offset += 3*3*channel_group[3][i][0];
			s->bias_offset->bias4_offset += channel_group[3][i][0];
			s->weight_offset->weight5_offset += input_channel[4]*channel_group[4][i][2];
			s->bias_offset->bias5_offset += channel_group[4][i][2];

			s->weight_offset->weight6_offset += 3*3*channel_group[5][i][0];
			s->bias_offset->bias6_offset += channel_group[5][i][0];
			s->weight_offset->weight7_offset += input_channel[6]*channel_group[6][i][2];
			s->bias_offset->bias7_offset += channel_group[6][i][2];

			s->weight_offset->weight8_offset += 3*3*channel_group[7][i][0];
			s->bias_offset->bias8_offset += channel_group[7][i][0];
			s->weight_offset->weight9_offset += input_channel[8]*channel_group[8][i][2];
			s->bias_offset->bias9_offset += channel_group[8][i][2];

			s->weight_offset->weight10_offset += 3*3*channel_group[9][i][0];
			s->bias_offset->bias10_offset += channel_group[9][i][0];
			s->weight_offset->weight11_offset += input_channel[10]*channel_group[10][i][2];
			s->bias_offset->bias11_offset += channel_group[10][i][2];

			s->weight_offset->weight12_offset += 3*3*channel_group[11][i][0];
			s->bias_offset->bias12_offset += channel_group[11][i][0];
			s->weight_offset->weight13_offset += input_channel[12]*channel_group[12][i][2];
			s->bias_offset->bias13_offset += channel_group[12][i][2];

			s->weight_offset->weight14_offset += 3*3*channel_group[13][i][0];
			s->bias_offset->bias14_offset += channel_group[13][i][0];
			s->weight_offset->weight15_offset += input_channel[13]*channel_group[13][i][2];
			s->bias_offset->bias15_offset += channel_group[13][i][2];

			s->weight_offset->weight16_offset += 3*3*channel_group[13][i][0];
			s->bias_offset->bias16_offset += channel_group[13][i][0];
			s->weight_offset->weight17_offset += input_channel[13]*channel_group[13][i][2];
			s->bias_offset->bias17_offset += channel_group[13][i][2];

			s->weight_offset->weight18_offset += 3*3*channel_group[13][i][0];
			s->bias_offset->bias18_offset += channel_group[13][i][0];
			s->weight_offset->weight19_offset += input_channel[13]*channel_group[13][i][2];
			s->bias_offset->bias19_offset += channel_group[13][i][2];

			s->weight_offset->weight20_offset += 3*3*channel_group[13][i][0];
			s->bias_offset->bias20_offset += channel_group[13][i][0];
			s->weight_offset->weight21_offset += input_channel[13]*channel_group[13][i][2];
			s->bias_offset->bias21_offset += channel_group[13][i][2];

			s->weight_offset->weight22_offset += 3*3*channel_group[13][i][0];
			s->bias_offset->bias22_offset += channel_group[13][i][0];
			s->weight_offset->weight23_offset += input_channel[13]*channel_group[13][i][2];
			s->bias_offset->bias23_offset += channel_group[13][i][2];

			s->weight_offset->weight24_offset += 3*3*channel_group[14][i][0];
			s->bias_offset->bias24_offset += channel_group[14][i][0];
			s->weight_offset->weight25_offset += input_channel[15]*channel_group[15][i][2];
			s->bias_offset->bias25_offset += channel_group[15][i][2];

			s->weight_offset->weight26_offset += 3*3*channel_group[16][i][0];
			s->bias_offset->bias26_offset += channel_group[16][i][0];
			s->weight_offset->weight27_offset += input_channel[17]*channel_group[17][i][2];
			s->bias_offset->bias27_offset += channel_group[17][i][2];

			s->weight_offset->weight28_offset += input_channel[18]*channel_group[18][i][2];
			s->bias_offset->bias28_offset += channel_group[18][i][2];
		}

			s->weight_offset->weight1_offset += 3*3*input_channel[0]*channel_group[0][0][0] * 8 * dspnum;
			s->bias_offset->bias1_offset += channel_group[0][0][0] * 8 * dspnum;

			s->weight_offset->weight2_offset += 3*3*channel_group[1][0][0] * 8 * dspnum;
			s->bias_offset->bias2_offset += channel_group[1][0][0] * 8 * dspnum;
			s->weight_offset->weight3_offset += input_channel[2]*channel_group[2][0][2] * 8 * dspnum;
			s->bias_offset->bias3_offset += channel_group[2][0][2] * 8 * dspnum;

			s->weight_offset->weight4_offset += 3*3*channel_group[3][0][0] * 8 * dspnum;
			s->bias_offset->bias4_offset += channel_group[3][0][0] * 8 * dspnum;
			s->weight_offset->weight5_offset += input_channel[4]*channel_group[4][0][2] * 8 * dspnum;
			s->bias_offset->bias5_offset += channel_group[4][0][2] * 8 * dspnum;

			s->weight_offset->weight6_offset += 3*3*channel_group[5][0][0] * 8 * dspnum;
			s->bias_offset->bias6_offset += channel_group[5][0][0] * 8 * dspnum;
			s->weight_offset->weight7_offset += input_channel[6]*channel_group[6][0][2] * 8 * dspnum;
			s->bias_offset->bias7_offset += channel_group[6][0][2] * 8 * dspnum;

			s->weight_offset->weight8_offset += 3*3*channel_group[7][0][0] * 8 * dspnum;
			s->bias_offset->bias8_offset += channel_group[7][0][0] * 8 * dspnum;
			s->weight_offset->weight9_offset += input_channel[8]*channel_group[8][0][2] * 8 * dspnum;
			s->bias_offset->bias9_offset += channel_group[8][0][2] * 8 * dspnum;

			s->weight_offset->weight10_offset += 3*3*channel_group[9][0][0] * 8 * dspnum;
			s->bias_offset->bias10_offset += channel_group[9][0][0] * 8 * dspnum;
			s->weight_offset->weight11_offset += input_channel[10]*channel_group[10][0][2] * 8 * dspnum;
			s->bias_offset->bias11_offset += channel_group[10][0][2] * 8 * dspnum;

			s->weight_offset->weight12_offset += 3*3*channel_group[11][0][0] * 8 * dspnum;
			s->bias_offset->bias12_offset += channel_group[11][0][0] * 8 * dspnum;
			s->weight_offset->weight13_offset += input_channel[12]*channel_group[12][0][2] * 8 * dspnum;
			s->bias_offset->bias13_offset += channel_group[12][0][2] * 8 * dspnum;

			s->weight_offset->weight14_offset += 3*3*channel_group[13][0][0] * 8 * dspnum;
			s->bias_offset->bias14_offset += channel_group[13][0][0] * 8 * dspnum;
			s->weight_offset->weight15_offset += input_channel[13]*channel_group[13][0][2] * 8 * dspnum;
			s->bias_offset->bias15_offset += channel_group[13][0][2] * 8 * dspnum;

			s->weight_offset->weight16_offset += 3*3*channel_group[13][0][0] * 8 * dspnum;
			s->bias_offset->bias16_offset += channel_group[13][0][0] * 8 * dspnum;
			s->weight_offset->weight17_offset += input_channel[13]*channel_group[13][0][2] * 8 * dspnum;
			s->bias_offset->bias17_offset += channel_group[13][0][2] * 8 * dspnum;

			s->weight_offset->weight18_offset += 3*3*channel_group[13][0][0] * 8 * dspnum;
			s->bias_offset->bias18_offset += channel_group[13][0][0] * 8 * dspnum;
			s->weight_offset->weight19_offset += input_channel[13]*channel_group[13][0][2] * 8 * dspnum;
			s->bias_offset->bias19_offset += channel_group[13][0][2] * 8 * dspnum;

			s->weight_offset->weight20_offset += 3*3*channel_group[13][0][0] * 8 * dspnum;
			s->bias_offset->bias20_offset += channel_group[13][0][0] * 8 * dspnum;
			s->weight_offset->weight21_offset += input_channel[13]*channel_group[13][0][2] * 8 * dspnum;
			s->bias_offset->bias21_offset += channel_group[13][0][2] * 8 * dspnum;

			s->weight_offset->weight22_offset += 3*3*channel_group[13][0][0] * 8 * dspnum;
			s->bias_offset->bias22_offset += channel_group[13][0][0] * 8 * dspnum;
			s->weight_offset->weight23_offset += input_channel[13]*channel_group[13][0][2] * 8 * dspnum;
			s->bias_offset->bias23_offset += channel_group[13][0][2] * 8 * dspnum;

			s->weight_offset->weight24_offset += 3*3*channel_group[14][0][0] * 8 * dspnum;
			s->bias_offset->bias24_offset += channel_group[14][0][0] * 8 * dspnum;
			s->weight_offset->weight25_offset += input_channel[15]*channel_group[15][0][2] * 8 * dspnum;
			s->bias_offset->bias25_offset += channel_group[15][0][2] * 8 * dspnum;

			s->weight_offset->weight26_offset += 3*3*channel_group[16][0][0] * 8 * dspnum;
			s->bias_offset->bias26_offset += channel_group[16][0][0] * 8 * dspnum;
			s->weight_offset->weight27_offset += input_channel[17]*channel_group[17][0][2] * 8 * dspnum;
			s->bias_offset->bias27_offset += channel_group[17][0][2] * 8 * dspnum;

			s->weight_offset->weight28_offset += input_channel[18]*channel_group[18][0][2] * 8 * dspnum;
			s->bias_offset->bias28_offset += channel_group[18][0][2] * 8 * dspnum;
	}
	return s;
}


void globalavg_pool_group(
	float* input,
	float* output,
	int layerno,
	int corenum
)
{

	int f_size;
	int img_h=img_height[layerno];
	int img_w=img_width[layerno];
	f_size = img_h * img_w;
	float avg,sum;
	int offset = channel_group[layerno][corenum][1];
	int out_c = channel_group[layerno][corenum][0];
	int co, j;
	for (co = 0; co < out_c ; co++) {
		sum = 0.0;
		avg = 0.0;
		for (j = 0; j < f_size; j++) {
			sum += input[(co+offset) * f_size + j];
		}
		avg = sum / f_size;
		output[co + offset] = avg;
	}
}

void dw_conv_3x3_inputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad,int step)
{
	if(step <= 11)
	{
		int rows_offset[4][29] = {{0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0 },
									  {0,0,0,0,0,
									   0,0,0,0,0,
									   0,1,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   1,0,0,0 },
									  {0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,0,0,
									   0,0,0,1,0,
									   1,0,0,0 },
									  {0,0,0,0,0,
									   0,0,0,0,0,
									   0,1,0,0,0,
									   0,0,0,0,0,
									   0,0,0,1,0,
									   1,0,0,0 },
		};
		float stride1 = 1.0f/stride;
	//    int img_h = img_height[layer_no];
	//    int img_w = img_width[layer_no];
		int img_h = img_tiling[step][DeviceId][1];//特别注意这个地方，img_tiling数组中的第2个位置的数据表示图像高度
		int img_w = img_tiling[step][DeviceId][0];//分块宽度

		int inp_c = channel_group[layer_no][inputgroupno][0];          //3x3通道分组大小
		int inp_c_offset = channel_group[layer_no][inputgroupno][1];   //3x3偏移量大小
		int co, h, w, m, n;
		float sum = 0.0, result;
		//int reimg = img_h - k_size + 2 * pad + stride;
		for (co = 0; co < inp_c; co++)
		{
			for (h = rows_offset[DeviceId][step]; h < img_h - (k_size - 1); h+=stride)
			{
				for (w = 0; w < img_w - (k_size - 1); w+=stride)
				{
					sum = 0.0;

					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							sum += weight[((co) * 3 + m) * 3 + n] *input[((co + inp_c_offset) * img_h  + (h + m)) * img_w + (w + n)];
						}
					}

					result = sum + bias[(co)];

					output[int((((co + inp_c_offset) * (img_h - (k_size-1)) * stride1) + h * stride1) * (img_w - (k_size-1)) * stride1 + w * stride1)] = (result < 0)? 0.0f : (result > 6) ? 6.0f : result;//2 = k-1

				}
			}
		}
	}else{
		 int img_h = img_height[layer_no];
		//int img_w = img_width[layer_no];

		int inp_c = channel_group[layer_no][inputgroupno][0];          //3x3通道分组大小
		int inp_c_offset = channel_group[layer_no][inputgroupno][1];   //3x3偏移量大小
		int co, h, w, m, n;
		float sum = 0.0, result;
		int reimg = img_h - k_size + 2 * pad + stride;
		for (co = 0; co < inp_c; co++)
		{
			for (h = 0; h < reimg; h+=stride)
			{
				for (w = 0; w < reimg; w+=stride)
				{
					sum = 0.0;

					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							sum += weight[((co) * 3 + m) * 3 + n] *
								((h + m - pad >= 0 && w + n - pad >= 0 && h + m - pad < reimg && w + n - pad < reimg) ? input[((co + inp_c_offset) * reimg + (h + m - pad)) * reimg + (w + n - pad)] : 0);
						}
					}

					result = sum + bias[(co)];

					if (result < 0)
					{
						output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = 0.0f;
					}
					else if (result > 6)
					{
						output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = 6.0f;
					}
					else
					{
						output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = result;
					}
				}
			}
		}
	}
}

void dw_conv_3x3_inputgroup_L21(float* input, float* weight, float* bias, float* output, int layer_no, int inputgroupno,int k_size, int stride, int pad)
{
    int img_h = img_height[layer_no];
    int img_w = img_width[layer_no];

    int inp_c = channel_group[layer_no][inputgroupno][0];          //3x3通道分组大小
    int inp_c_offset = channel_group[layer_no][inputgroupno][1];   //3x3偏移量大小
    int co, h, w, m, n;
    float sum = 0.0, result;
    int reimg = img_h - k_size + 2 * pad + stride;
    for (co = 0; co < inp_c; co++)
    {
        for (h = 0; h < reimg; h+=stride)
        {
            for (w = 0; w < reimg; w+=stride)
            {
//                sum = 0.0;
//
//                for (m = 0; m < 3; m++)
//                {
//                    for (n = 0; n < 3; n++)
//                    {
//                        sum += weight[((co) * 3 + m) * 3 + n] *
//                            ((h + m - pad >= 0 && w + n - pad >= 0 && h + m - pad < reimg && w + n - pad < reimg) ? input[((co + inp_c_offset) * reimg + (h + m - pad)) * reimg + (w + n - pad)] : 0);
//                    }
//                }
//
//                result = sum + bias[(co)];
//
//                output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = (result < 0) ? 0.0f : (result > 6) ? 6.0f : result;
                if (output[(((co + inp_c_offset) * reimg) + h) * reimg + w] < 0)
                {
                    output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = 0.0f;
                }
                else if (output[(((co + inp_c_offset) * reimg) + h) * reimg + w] > 6)
                {
                    output[(((co + inp_c_offset) * reimg) + h) * reimg + w] = 6.0f;
                }
                else
                {
                    ;
                }
            }
        }
    }
}

void concat(float* input1,float* input2,float* output){

	memcpy(output,input1,sizeof(input1)/sizeof(input1[0])*sizeof(float));
	memcpy(&output[sizeof(input1)/sizeof(input1[0])],input2,sizeof(input2)/sizeof(input2[0])*sizeof(float));
}

#if CODE_OPTIMIZATION
void conv_3x3_outputgroup_L2(
	float* input,
	float* weight,
	float* bias,
	float* output,
	int layer_no,
	int inputgroupno,
	int stride,
	int step
) {
	 float stride1 = 1.0f/stride;
		int inp_c = input_channel[layer_no];
	//	int img_h = img_height[layer_no];
	//	int img_w = img_width[layer_no];
		int img_h = img_tiling[step][DeviceId][1];//特别注意这个地方，img_tiling数组中的第2个位置的数据表示图像高度
		int img_w = img_tiling[step][DeviceId][0];//分块宽度
		int out_c = channel_group[layer_no][inputgroupno][0];          //3x3通道分组大小
		int out_c_offset = channel_group[layer_no][inputgroupno][1];   //3x3偏移量大小
		float sum, result;
		int co, h, w;
		int count ;
		//float e = 0.00001;
		for (co = 0; co < out_c; co++) {
			count = 0;
			for (h = 0; h < img_h - 2; h += stride) {
				for (w = 0; w < img_w - 2; w += stride) {
					sum = 0.0;
					sum = DSPF_sp_dotprod(weight + co * inp_c * 3 * 3, input + count * inp_c * 3 * 3, inp_c * 3 * 3);
					count++;
					result = sum + bias[(co)];
					output[int((((co + out_c_offset) * (img_h - 2)*stride1) + h * stride1) * (img_w - 2)* stride1 + w*stride1)] = (result < 0)? 0.0f : (result > 6) ? 6.0f : result;
				}
			}
		}
}
#else
void conv_3x3_outputgroup_L2(
	float* input,
	float* weight,
	float* bias,
	float* output,
	int layer_no,
	int inputgroupno,
	int stride
) {
	float stride1 = 1.0f/stride;
	int co, ci, m, n, h, w;
	float result, sum;
	int inp_c = input_channel[layer_no];
	int img_h = img_height[layer_no];
	int img_w = img_width[layer_no];
	int out_c = channel_group[layer_no][inputgroupno][0];          //3x3通道分组大小
	int out_c_offset = channel_group[layer_no][inputgroupno][1];   //3x3偏移量大小
	for (co = 0; co < out_c; co++) {
		for (h = 0; h < img_h; h+=stride) {
			for (w = 0; w < img_w; w+=stride) {
				sum = 0.0;
				for (ci = 0; ci < inp_c; ci++) {
					for (m = 0; m < 3; m++) {
						for (n = 0; n < 3; n++) {

							sum += weight[co*inp_c * 3 * 3 + ((ci) * 3 + m) * 3 + n] *
								((h + m - 1 >= 0 && w + n - 1 >= 0 && h + m - 1 < img_h && w + n - 1 < img_w) ? input[((ci)* img_h + (h + m - 1)) * img_w + (w + n - 1)] : 0);
						}
					}
				}
				result = sum + bias[(co)];
				output[int((((co + out_c_offset) * img_h*stride1) + h*stride1) * img_w * stride1 + w * stride1)] = (result < 0)? 0.0f:result;
			}
		}
	}
//	int inp_c = input_channel[layer_no];
//	int img_h = img_height[layer_no];
//	int img_w = img_width[layer_no];
//	int out_c = channel_group[layer_no][inputgroupno][2];          //3x3通道分组大小
//	int out_c_offset = channel_group[layer_no][inputgroupno][3];   //3x3偏移量大小
//	float sum,result;
//	float sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, sum9;
//	int co, ci, h, w;
//	for (co = 0; co < out_c; co++) {
//		for (h = 0; h < img_h; h++) {
//			for (w = 0; w < img_w; w++) {
//				sum = 0.0;
//				sum1 = 0.0;
//				sum2 = 0.0;
//				sum3 = 0.0;
//				sum4 = 0.0;
//				sum5 = 0.0;
//				sum6 = 0.0;
//				sum7 = 0.0;
//				sum8 = 0.0;
//				sum9 = 0.0;
//				for (ci = 0; ci < inp_c; ci++) {
//					sum1 += weight[co*inp_c * 3 * 3 + ((ci) * 3) * 3] * ((h - 1 >= 0 && w - 1 >= 0 && h - 1 < img_h && w - 1 < img_w) ? input[((ci)* img_h + (h - 1)) * img_w + (w - 1)] : 0);
//					sum2 += weight[co*inp_c * 3 * 3 + ((ci) * 3) * 3 + 1] * ((h - 1 >= 0 && w >= 0 && h - 1 < img_h && w < img_w) ? input[((ci)* img_h + (h - 1)) * img_w + (w)] : 0);
//					sum3 += weight[co*inp_c * 3 * 3 + ((ci) * 3) * 3 + 2] * ((h - 1 >= 0 && w + 1 >= 0 && h - 1 < img_h && w + 1 < img_w) ? input[((ci)* img_h + (h - 1)) * img_w + (w + 1)] : 0);
//					sum4 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 1) * 3] * ((h >= 0 && w - 1 >= 0 && h < img_h && w - 1 < img_w) ? input[((ci)* img_h + (h)) * img_w + (w- 1)] : 0);
//					sum5 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 1) * 3 + 1] *((h >= 0 && w >= 0 && h < img_h && w < img_w) ? input[((ci)* img_h + (h)) * img_w + (w)] : 0);
//					sum6 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 1) * 3 + 2] * ((h >= 0 && w + 1 >= 0 && h < img_h && w + 1 < img_w) ? input[((ci)* img_h + (h)) * img_w + (w + 1)] : 0);
//					sum7 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 2) * 3] * ((h + 1 >= 0 && w - 1 >= 0 && h + 1 < img_h && w - 1 < img_w) ? input[((ci)* img_h + (h + 1)) * img_w + (w - 1)] : 0);
//					sum8 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 2) * 3 + 1] * ((h + 1 >= 0 && w >= 0 && h + 1 < img_h && w < img_w) ? input[((ci)* img_h + (h + 1)) * img_w + (w)] : 0);
//					sum9 += weight[co*inp_c * 3 * 3 + ((ci) * 3 + 2) * 3 + 2] * ((h + 1 >= 0 && w + 1 >= 0 && h + 1 < img_h && w + 1 < img_w) ? input[((ci)* img_h + (h + 1)) * img_w + (w + 1)] : 0);
//				}
//				sum = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8 + sum9;
//				result = sum + bias[(co)];
//				output[(((co + out_c_offset) * img_h) + h) * img_w + w] = (result < 0)? 0.0f:result;
//			}
//		}
//	}
}
#endif


#if CODE_OPTIMIZATION
void dw_conv_1x1_outputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int outputgroupno, int stride,int step)
{
	if(step <= 11)
	{
		float stride1 = 1.0f/stride;
		int inp_c = input_channel[layer_no];
		//int img_h = img_height[layer_no]/4;
		int img_h = img_tiling[step][DeviceId][1];//特别注意这个地方，img_tiling数组中的第2个位置的数据表示图像高度
		int img_w = img_width[layer_no];
		int co, h, w;
		int out_c = channel_group[layer_no][outputgroupno][2];          //1x1通道分组大小
		int out_c_offset = channel_group[layer_no][outputgroupno][3];   //1x1偏移量大小
		int output_total = output_channel[layer_no];
		float sum, result;
		//float e = 0.00001;
		for (co = 0; co < out_c; co++)
		{
			for (h = 0; h < img_h; h+=stride)
			{
				for (w = 0; w < img_w; w+=stride)
				{
					sum = DSPF_sp_dotprod(weight + co * inp_c, input + h * img_w * inp_c + w * inp_c, inp_c);
					result = sum + bias[(co)];
					output[int(h*stride1 * img_w*stride1 * output_total + w*stride1 * output_total + co + out_c_offset)] = (result < 0) ? 0.0f : (result > 6) ? 6.0f : result;//
				}
			}
		}
	}else{
		float stride1 = 1.0f/stride;
		int inp_c = input_channel[layer_no];
		int img_h = img_height[layer_no];
		int img_w = img_width[layer_no];
		int co, h, w;
		int out_c = channel_group[layer_no][outputgroupno][2];          //1x1通道分组大小
		int out_c_offset = channel_group[layer_no][outputgroupno][3];   //1x1偏移量大小
		int output_total = output_channel[layer_no];
		float sum, result;
		//float e = 0.00001;
		for (co = 0; co < out_c; co++)
		{
			for (h = 0; h < img_h; h+=stride)
			{
				for (w = 0; w < img_w; w+=stride)
				{
					sum = DSPF_sp_dotprod(weight + co * inp_c, input + h * img_w * inp_c + w * inp_c, inp_c);
					result = sum + bias[(co)];
					output[int(h*stride1 * img_w*stride1 * output_total + w*stride1 * output_total + co + out_c_offset)] = (result < 0) ? 0.0f : (result > 6) ? 6.0f : result;

					//output[int(h*stride1 * img_w*stride1 * output_total + w*stride1 * output_total + co + out_c_offset)] = ((output[int(h*stride1 * img_w*stride1 * output_total + w*stride1 * output_total + co + out_c_offset)] - 1.0) / sqrt(1.0 + e)) * 0.5 + 0.5;
				}
			}
		}
	}
}
#else
void dw_conv_1x1_outputgroup_L2(float* input, float* weight, float* bias, float* output, int layer_no, int outputgroupno, int stride)
{
	float stride1 = 1.0f/stride;
    int inp_c = input_channel[layer_no];
    int img_h = img_height[layer_no];
    int img_w = img_width[layer_no];
    int co, h, ci, w;
    int out_c = channel_group[layer_no][outputgroupno][2];          //1x1通道分组大小
    int out_c_offset = channel_group[layer_no][outputgroupno][3];   //1x1偏移量大小
    float sum, result;

    for (co = 0; co < out_c; co++)
    {
        for (h = 0; h < img_h; h+=stride)
        {
            for (w = 0; w < img_w; w+=stride)
            {
                sum = 0.0;
                for (ci = 0; ci < inp_c; ci++)
                {
                    sum += weight[(co) * inp_c + ci] * input[(ci * img_h + h) * img_w + w];
                }
                result = sum + bias[(co)];

                output[int(((co + out_c_offset) * img_h*stride1 + h*stride1) * img_w*stride1 + w*stride1)] = result;
            }
        }
    }
}
#endif



//int SrioSendData(uint32_t LocalDeviceID, uint32_t DstDeviceId, uint32_t LocalAddr, uint32_t DstAddr, uint32_t DataLen)
//{
//	int a;
//	return a;
//}



//中间块补齐（处理中间分块上边界扩边需要的数据）
void tiling_input_1_n_from_tiled_sram_infront( float* tiling_input_1_n, float* output_n, int rows, int weight, int height_1_n, int ch)
{
    int num_infront = rows;
    int num_behind = rows;
    /*************************************/
    int offset = 0;
    int offset0 = 0;
    int k;

	for (k = 0; k < ch; k++)
	{
		offset0 = k * weight * height_1_n + weight * (height_1_n - num_infront);
		int size = num_infront * weight;
		memcpy(output_n + offset, tiling_input_1_n + offset0, size * sizeof(float));
		offset += size;
	}
}

//第一个块补齐（处理第1个分块上边界pading和下边界扩边需要的数据）
void tiling_input_0_from_tiled_sram(float* tiling_input_n_1, float* output_n, int rows, int weight, int height_n_1, int ch)
{
	memset(output_n, 0x00, ch * rows * weight * sizeof(float));
}

//中间块补齐（处理中间分块下边界扩边需要的数据）
void tiling_input_1_n_from_tiled_sram_behind(float* tiling_input_n_1, float* output_n,int rows, int weight, int height_n_1, int ch)
{
    int num_behind = rows;
    int num_infront = rows;
    /*************************************/
    int offset0 = 0;
    int offset_temp = 0;
    int k;

	for (k = 0; k < ch; k++)
	{
		offset0 = k * height_n_1 * weight;
		int size = num_behind * weight;
		memcpy(output_n + offset_temp, tiling_input_n_1 + offset0, size * sizeof(float));
		offset_temp += size;
		offset0 += size;
	}
}
//int SrioSendData(uint32_t LocalDeviceID, uint32_t DstDeviceId, uint32_t LocalAddr, uint32_t DstAddr, uint32_t DataLen)
void writedata( float* tiling_input, float** tiling_buffer_read, float** tiling_buffer_write, int rows, int weight, int height_1_n, int ch)
{
#if ring
	switch (DeviceId)
	{
		case 0:
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1], rows, weight, height_1_n, ch/8);
			tiling_input_0_from_tiled_sram(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);
			SrioSendData(DeviceId, 3, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, DeviceId + 1, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
			break;
		case 1:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0], rows, weight, height_1_n, ch/8);
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);
			SrioSendData(DeviceId, DeviceId - 1, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, DeviceId + 1, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
			break;
		case 2:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);
			SrioSendData(DeviceId, DeviceId - 1, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, DeviceId + 1, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
			break;
		case 3:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);
			tiling_input_0_from_tiled_sram(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);
			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, DeviceId - 1, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			break;
		default:break;
	}
#endif

#if ps
	switch (DeviceId)
	{
		case 0:
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1], rows, weight, height_1_n, ch/8);//给1的
			tiling_input_0_from_tiled_sram(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);//给3的全0

			SrioSendData(DeviceId, 3, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, 1, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
			break;
		case 1:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0], rows, weight, height_1_n, ch/8);
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);

			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[3] ,  rows * weight * ch * 4);
			break;
		case 2:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);
			tiling_input_1_n_from_tiled_sram_infront(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);

			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[4] ,  rows * weight * ch * 4);
			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[5] ,  rows * weight * ch * 4);
			break;
		case 3:
			tiling_input_1_n_from_tiled_sram_behind(tiling_input, tiling_buffer_write[0],  rows, weight, height_1_n, ch/8);
			tiling_input_0_from_tiled_sram(tiling_input, tiling_buffer_write[1],  rows, weight, height_1_n, ch/8);

			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[1], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);//全0给dsp0穿
			SrioSendData(DeviceId, 0, (uint32_t)tiling_buffer_write[0], (uint32_t)tiling_buffer_read[2] ,  rows * weight * ch * 4);
			break;
		default:break;
	}


	if(DeviceId == 0)
	{
		while(1)
		{
			InvalidCache((void *)&RecvNum, 4);
			if(RecvNum == 6)
			{
				RecvNum -= 6;
				//printf("%d:; ",RecvNum);
				break;
			}
		}
		SrioSendData(DeviceId, 1, (uint32_t)tiling_buffer_read[4], (uint32_t)tiling_buffer_read[1] , rows * weight * ch * 4);
		SrioSendData(DeviceId, 2, (uint32_t)tiling_buffer_read[3], (uint32_t)tiling_buffer_read[0] , rows * weight * ch * 4);
		SrioSendData(DeviceId, 2, (uint32_t)tiling_buffer_read[2], (uint32_t)tiling_buffer_read[1] , rows * weight * ch * 4);
		SrioSendData(DeviceId, 3, (uint32_t)tiling_buffer_read[5], (uint32_t)tiling_buffer_read[0] , rows * weight * ch * 4);
	}

	if(DeviceId != 0)
	{
		while(1)
		{
			InvalidCache((void *)&RecvNum, 4);
			if(RecvNum == 2)
			{
				RecvNum -= 2;
				//printf("%d:; ",RecvNum);
				break;
			}
		}
	}

//	if(DeviceId == 0)
//	{
//		while(1)
//		{
//			//判断DSP1-3给0 传输结束
//			break;
//		}
//		SrioSendData(DeviceId, 1, (uint32_t)tiling_buffer_read[4], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
//		SrioSendData(DeviceId, 2, (uint32_t)tiling_buffer_read[3], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
//		SrioSendData(DeviceId, 2, (uint32_t)tiling_buffer_read[2], (uint32_t)tiling_buffer_read[1] ,  rows * weight * ch * 4);
//		SrioSendData(DeviceId, 3, (uint32_t)tiling_buffer_read[5], (uint32_t)tiling_buffer_read[0] ,  rows * weight * ch * 4);
//	}
//	if(DeviceId != 0)
//	{
//		while(1)
//		{
//			//判断0给DSP1-3 传输结束
//			break;
//		}
//	}
#endif
}


void readdata( float* tiling_input, float** tiling_buffer_read, float* tiling_output, int rows, int weight, int height_1_n, int ch)
{
	switch (DeviceId)
	{
		case 0:
			well_aligned_data_sram(tiling_input, tiling_buffer_read, tiling_output, rows, weight, height_1_n, ch);
			break;
		case 1:
			well_aligned_data_sram(tiling_input, tiling_buffer_read, tiling_output, rows, weight, height_1_n, ch);
			break;
		case 2:
			well_aligned_data_sram(tiling_input, tiling_buffer_read, tiling_output, rows, weight, height_1_n, ch);
			break;
		case 3:
			well_aligned_data_sram(tiling_input, tiling_buffer_read, tiling_output, rows, weight, height_1_n, ch);
			break;
		default:break;
	}
}

//void well_aligned_data_sram(float* input, float** input_sram, float* output, int rows, int weight, int org_height, int ch)
//{
//    int i, j, k;
//    int height = org_height + rows + rows;
//    int offset = 0;
//    int offset_add0 = 0;
//    int offset_add1 = 0;
//
//    for (k = 0; k < ch; k++)
//    {
//        for (i = 0; i < height; i++)
//        {
//            output[k * height * (weight + 2) + i * (weight + 2)] = 0;
//            for (j = 1; j < (weight + 1); j++)
//            {
//				//bool ex = i < num_infront || i >= (num_infront + org_height);
//				if(i < rows)
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] =  input_sram[0][offset_add0];
//					offset_add0++;
//
//				}
//				else if(i >= (rows + org_height))
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] = input_sram[1][offset_add1];
//					offset_add1++;
//				}
//				else
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] = input[offset];
//					offset++;
//				}
//            }
//            output[k * height * (weight + 2) + i * (weight + 2) + (weight + 1)] = 0;
//        }
//    }
//}

void well_aligned_data_sram(float* input, float** input_sram, float* output, int rows, int weight, int org_height, int ch)
{
    int i, j, k;
    int height = org_height + rows + rows;
    int offset = 0;
    int offset_add0 = 0;
    int offset_add1 = 0;

    for (k = 0; k < ch; k++)
    {
        for (i = 0; i < height; i++)
        {
            output[k * height * (weight + 2) + i * (weight + 2)] = 0;
//            for (j = 1; j < (weight + 1); j++)
//            {
//				//bool ex = i < num_infront || i >= (num_infront + org_height);
//				if(i < rows)
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] =  input_sram[0][offset_add0];
//					offset_add0++;
//
//				}
//				else if(i >= (rows + org_height))
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] = input_sram[1][offset_add1];
//					offset_add1++;
//				}
//				else
//				{
//					output[k * height * (weight + 2) + i * (weight + 2) + j] = input[offset];
//					offset++;
//				}
//            }
            if(i < rows)
			{
            	memcpy(output + k * height * (weight + 2) + i * (weight + 2), input_sram[0] + offset_add0, weight * sizeof(float));
				//output[k * height * (weight + 2) + i * (weight + 2) + j] =  input_sram[0][offset_add0];
				offset_add0 += weight;

			}
			else if(i >= (rows + org_height))
			{
				memcpy(output + k * height * (weight + 2) + i * (weight + 2), input_sram[1] + offset_add1, weight * sizeof(float));
				//output[k * height * (weight + 2) + i * (weight + 2) + j] = input_sram[1][offset_add1];
				offset_add1 += weight;
			}
			else
			{
				memcpy(output + k * height * (weight + 2) + i * (weight + 2), input + offset, weight * sizeof(float));
				//output[k * height * (weight + 2) + i * (weight + 2) + j] = input[offset];
				offset += weight;
			}




            output[k * height * (weight + 2) + i * (weight + 2) + (weight + 1)] = 0;
        }
    }
}


//处理第一个分块
void tiling_input_0(float* img, float* output, int offset0, int layer_no, int tiling_no)
{
    int i;
    int offset = offset0;
    const int pos0 = img_tiling[layer_no][tiling_no][0];//226
    const int pos1 = img_tiling[layer_no][tiling_no][1];//58
    const int channel = input_channel[layer_no];        //3

    for (i = 0; i < pos0 * pos1 * channel; i++)
    {
        if (i%(pos0 * pos1)< pos0)  //判断是否是三个通道的第一行
        {
            output[i] = 0; //3个通道的第1行数据为零
        }
        else if (i % pos0 == 0 || (i+1) % pos0 == 0)
        {
            output[i] = 0; //3个通道的第1个数据和最后1个数据点为零
        }
        else
        {
            output[i] = img[offset];
            offset++;
        }
        if ((i + 1) % (pos0 * pos1) == 0)//第一个通道中的第一块，第2个通道中的第一块...最后一个通洞中的第一块
        {
            offset += img_height[layer_no] * img_width[layer_no] - (pos1 - 1) * img_width[layer_no];
        }
    }
}

//处理最后一个分块
void tiling_input_n(float* img, float* output, int offset, int layer_no, int tiling_no)
{
    int i = 0;
    for (i = 0; i < img_tiling[layer_no][tiling_no][0] * img_tiling[layer_no][tiling_no][1] * input_channel[layer_no]; i++)
    {
        if ((i+img_tiling[layer_no][tiling_no][0])% (img_tiling[layer_no][tiling_no][0] * img_tiling[layer_no][tiling_no][1])< img_tiling[layer_no][tiling_no][0])
        {
            output[i] = 0; //最后一个分块，每一个通道的最后一行为零
        }
        else if (i % img_tiling[layer_no][tiling_no][0] == 0 || (i - img_tiling[layer_no][tiling_no][0] + 1) % img_tiling[layer_no][tiling_no][0] == 0)
        {
            output[i] = 0; //3个通道的第1个数据和最后1个数据点为零
        }
        else
        {
            output[i] = img[offset];
            offset++;
        }
        if ((i + 1) % (img_tiling[layer_no][tiling_no][0] * img_tiling[layer_no][tiling_no][1]) == 0)
        {
            offset += img_height[layer_no] * img_width[layer_no] - (img_tiling[layer_no][tiling_no][1]-1) * img_width[layer_no];
        }
    }
}

//除了第一块和最后一块，处理中间分块
void tiling_input_1_n(float* img, float* output, int offset, int layer_no, int tiling_no)
{
    int i;
    for (i = 0; i < img_tiling[layer_no][tiling_no][0] * img_tiling[layer_no][tiling_no][1] * input_channel[layer_no]; i++)
    {
        if (i % img_tiling[layer_no][tiling_no][0] == 0 || (i + 1) % img_tiling[layer_no][tiling_no][0] == 0)
        {
            output[i] = 0; //3个通道的第1个数据和最后1个数据点为零
        }
        else
        {
            output[i] = img[offset];
            offset++;
        }
        if ((i + 1) % (img_tiling[layer_no][tiling_no][0] * img_tiling[layer_no][tiling_no][1]) == 0)
        {
            offset = offset + img_height[layer_no] * img_width[layer_no] - (img_tiling[layer_no][tiling_no][1]) * img_width[layer_no];
            offset = offset;
        }
    }
}
void tiling_to_big(float* input, float* output, int layer_no, int tiling_no)
{
    //printf("dw%d conv 1x1 start!\n", layer_no + 1);
    int out_c = output_channel[layer_no]/8;//////////////////////////////分给8个core
    int img_h = img_tiling[layer_no][tiling_no][1] - 2;//特别注意这个地方，img_tiling数组中的第2个位置的数据表示图像高度，为分块大小的高度
    int img_w = img_tiling[layer_no][tiling_no][0] - 2;
    int img_h_offset = img_tiling[layer_no][tiling_no][2];
    //int h, co, w;
    int co;

    for (co = 0; co < out_c; co++)
    {
        //for (h = 0; h < img_h; h++)
        //{
        //    for (w = 0; w < img_w; w++)
        //    {
        //        //写入到大图中：偏移量图像高度为整个图像高度
        //        output[co * img_height[layer_no] * img_w + img_h_offset * img_w + h * img_w + w] = input[(co * img_h + h) * img_w + w];
        //    }
        //}
    	memcpy(output + co * img_height[layer_no] * img_w + img_h_offset * img_w, input + co * img_h * img_w, img_w * img_h * sizeof(float));
    }
}
