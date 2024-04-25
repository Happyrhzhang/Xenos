#include "datatrans.h"

void LoadWeightBiasFromDDRToL2Channel_r1(sensenet_t* s)
{
	memcpy(s->weight->weight1, s->weight_ddr->weight1_DDR + s->weight_offset->weight1_offset, 3*3*input_channel[0]*channel_group[0][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias1, s->bias_ddr->bias1_DDR + s->bias_offset->bias1_offset, channel_group[0][s->ui_core][0] * sizeof(float));

	memcpy(s->weight->weight2, s->weight_ddr->weight2_DDR + s->weight_offset->weight2_offset, 3*3*channel_group[1][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias2, s->bias_ddr->bias2_DDR + s->bias_offset->bias2_offset, channel_group[1][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight3, s->weight_ddr->weight3_DDR + s->weight_offset->weight3_offset, input_channel[2]*channel_group[2][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias3, s->bias_ddr->bias3_DDR + s->bias_offset->bias3_offset, channel_group[2][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight4, s->weight_ddr->weight4_DDR + s->weight_offset->weight4_offset, 3*3*channel_group[3][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias4, s->bias_ddr->bias4_DDR + s->bias_offset->bias4_offset, channel_group[3][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight5, s->weight_ddr->weight5_DDR + s->weight_offset->weight5_offset, input_channel[4]*channel_group[4][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias5, s->bias_ddr->bias5_DDR + s->bias_offset->bias5_offset, channel_group[4][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight6, s->weight_ddr->weight6_DDR + s->weight_offset->weight6_offset, 3*3*channel_group[5][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias6, s->bias_ddr->bias6_DDR + s->bias_offset->bias6_offset, channel_group[5][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight7, s->weight_ddr->weight7_DDR + s->weight_offset->weight7_offset, input_channel[6]*channel_group[6][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias7, s->bias_ddr->bias7_DDR + s->bias_offset->bias7_offset, channel_group[6][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight8, s->weight_ddr->weight8_DDR + s->weight_offset->weight8_offset, 3*3*channel_group[7][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias8, s->bias_ddr->bias8_DDR + s->bias_offset->bias8_offset, channel_group[7][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight9, s->weight_ddr->weight9_DDR + s->weight_offset->weight9_offset, input_channel[8]*channel_group[8][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias9, s->bias_ddr->bias9_DDR + s->bias_offset->bias9_offset, channel_group[8][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight10, s->weight_ddr->weight10_DDR + s->weight_offset->weight10_offset, 3*3*channel_group[9][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias10, s->bias_ddr->bias10_DDR + s->bias_offset->bias10_offset, channel_group[9][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight11, s->weight_ddr->weight11_DDR + s->weight_offset->weight11_offset, input_channel[10]*channel_group[10][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias11, s->bias_ddr->bias11_DDR + s->bias_offset->bias11_offset, channel_group[10][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight12, s->weight_ddr->weight12_DDR + s->weight_offset->weight12_offset, 3*3*channel_group[11][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias12, s->bias_ddr->bias12_DDR + s->bias_offset->bias12_offset, channel_group[11][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight13, s->weight_ddr->weight13_DDR + s->weight_offset->weight13_offset, input_channel[12]*channel_group[12][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias13, s->bias_ddr->bias13_DDR + s->bias_offset->bias13_offset, channel_group[12][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight14, s->weight_ddr->weight14_DDR + s->weight_offset->weight14_offset, 3*3*channel_group[13][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias14, s->bias_ddr->bias14_DDR + s->bias_offset->bias14_offset, channel_group[13][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight15, s->weight_ddr->weight15_DDR + s->weight_offset->weight15_offset, input_channel[13]*channel_group[13][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias15, s->bias_ddr->bias15_DDR + s->bias_offset->bias15_offset, channel_group[13][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight16, s->weight_ddr->weight16_DDR + s->weight_offset->weight16_offset, 3*3*channel_group[13][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias16, s->bias_ddr->bias16_DDR + s->bias_offset->bias16_offset, channel_group[13][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight17, s->weight_ddr->weight17_DDR + s->weight_offset->weight17_offset, input_channel[13]*channel_group[13][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias17, s->bias_ddr->bias17_DDR + s->bias_offset->bias17_offset, channel_group[13][s->ui_core][2] * sizeof(float));
}

void LoadWeightBiasFromDDRToL2Channel_r2(sensenet_t* s)
{
	memcpy(s->weight->weight18, s->weight_ddr->weight18_DDR + s->weight_offset->weight18_offset, 3*3*channel_group[13][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias18, s->bias_ddr->bias18_DDR + s->bias_offset->bias18_offset, channel_group[13][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight19, s->weight_ddr->weight19_DDR + s->weight_offset->weight19_offset, input_channel[13]*channel_group[13][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias19, s->bias_ddr->bias19_DDR + s->bias_offset->bias19_offset, channel_group[13][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight20, s->weight_ddr->weight20_DDR + s->weight_offset->weight20_offset, 3*3*channel_group[13][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias20, s->bias_ddr->bias20_DDR + s->bias_offset->bias20_offset, channel_group[13][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight21, s->weight_ddr->weight21_DDR + s->weight_offset->weight21_offset, input_channel[13]*channel_group[13][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias21, s->bias_ddr->bias21_DDR + s->bias_offset->bias21_offset, channel_group[13][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight22, s->weight_ddr->weight22_DDR + s->weight_offset->weight22_offset, 3*3*channel_group[13][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias22, s->bias_ddr->bias22_DDR + s->bias_offset->bias22_offset, channel_group[13][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight23, s->weight_ddr->weight23_DDR + s->weight_offset->weight23_offset, input_channel[13]*channel_group[13][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias23, s->bias_ddr->bias23_DDR + s->bias_offset->bias23_offset, channel_group[13][s->ui_core][2] * sizeof(float));
}

void LoadWeightBiasFromDDRToL2Channel_r3(sensenet_t* s)
{
	memcpy(s->weight->weight24, s->weight_ddr->weight24_DDR + s->weight_offset->weight24_offset, 3*3*channel_group[14][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias24, s->bias_ddr->bias24_DDR + s->bias_offset->bias24_offset, channel_group[14][s->ui_core][0] * sizeof(float));
	memcpy(s->weight->weight25, s->weight_ddr->weight25_DDR + s->weight_offset->weight25_offset, input_channel[15]*channel_group[15][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias25, s->bias_ddr->bias25_DDR + s->bias_offset->bias25_offset, channel_group[15][s->ui_core][2] * sizeof(float));

	memcpy(s->weight->weight26, s->weight_ddr->weight26_DDR + s->weight_offset->weight26_offset, 3*3*channel_group[16][s->ui_core][0]* sizeof(float));
	memcpy(s->bias->bias26, s->bias_ddr->bias26_DDR + s->bias_offset->bias26_offset, channel_group[16][s->ui_core][0] * sizeof(float));
}

void LoadWeightBiasFromDDRToL2Channel_r4(sensenet_t* s)
{
	memcpy(s->weight->weight27, s->weight_ddr->weight27_DDR + s->weight_offset->weight27_offset, input_channel[17]*channel_group[17][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias27, s->bias_ddr->bias27_DDR + s->bias_offset->bias27_offset, channel_group[17][s->ui_core][2] * sizeof(float));
}

void LoadWeightBiasFromDDRToL2Channel_r5(sensenet_t* s)
{
	memcpy(s->weight->weight28, s->weight_ddr->weight28_DDR + s->weight_offset->weight28_offset, input_channel[18]*channel_group[18][s->ui_core][2]* sizeof(float));
	memcpy(s->bias->bias28, s->bias_ddr->bias28_DDR + s->bias_offset->bias28_offset, channel_group[18][s->ui_core][2] * sizeof(float));
}

void LoadWeightBiasFromDDRToL2Channel_r6(sensenet_t* s)
{

}

void LoadWeightBiasFromDDRToL2Channel_r7(sensenet_t* s)
{

}

void LoadWeightBiasFromDDRToL2Channel_r8(sensenet_t* s)
{

}

