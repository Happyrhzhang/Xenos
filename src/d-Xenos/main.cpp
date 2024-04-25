#include "datatrans.h" //datatrans-sensenet-common/commondef

void core_work();
extern "C" {
#include "SRIO/SRIO_Test.h"
}

extern volatile Uint32 RecvNum;
int main()
{
	uint32_t ui_core_num = CSL_chipReadReg(CSL_CHIP_DNUM);

	if(ui_core_num == 0)
	{
		KeyStone_common_CPU_init();
		/*print device information.
		Enable memory protection interrupts, EDC for MSMC RAM*/
//		KeyStone_common_device_init();

		//enable exception handling
		KeyStone_Exception_cfg(TRUE);

#if 1 	/*for debug only. Without these code you need reset DSP before your rerun the program*/
	//disable SRIO through PSC
	KeyStone_disable_PSC_module(CSL_PSC_PD_SRIO, CSL_PSC_LPSC_SRIO);
	KeyStone_disable_PSC_Power_Domain(CSL_PSC_PD_SRIO);

#endif

		SRIOInit(DeviceId);
	}




	if (ui_core_num == 0)
	{
		CACHE_setL1PSize(CACHE_L1_32KCACHE);
		CACHE_setL1DSize(CACHE_L1_32KCACHE);
		CACHE_setL2Size(CACHE_0KCACHE);
		CACHE_invAllL1p(CACHE_WAIT);
		//CGEM_regs->MAR[179]=0;
	    int i;
	    for(i = 128;i<256;i++){
	    	CACHE_enableCaching(i);
	    }
	    CACHE_setMemRegionInfo(12,1,1);
	}
	else
	{
		CACHE_setL1PSize(CACHE_L1_32KCACHE);
		CACHE_setL1DSize(CACHE_L1_32KCACHE);
		CACHE_setL2Size(CACHE_0KCACHE);
		CACHE_invAllL1p(CACHE_WAIT);
//		//CGEM_regs->MAR[179]=0;
	    int i;
	    for(i = 128;i<256;i++){
	    	CACHE_enableCaching(i);
	    }
	    CACHE_setMemRegionInfo(12,1,1);
	}
	CACHE_disableCaching(138);
	core_work();
    return 0;
}
void core_work()
{
	int i_frame = 0;
	int step = 0;

#if CALC_TOTAL_TIME
    long long StartTime = 0;
    long long EndTime = 0;
    TSCH = 0;
    TSCL = 0;
#endif

    uint32_t ui_core_num = CSL_chipReadReg(CSL_CHIP_DNUM);
    sensenet_t* s = sensenet_open(ui_core_num);

	if (ui_core_num == 0)
	{
		printf("core 0 ready!\n");
	}

	//for (i_frame = 0; true; i_frame++)
	{
		for (step = 0; step < 29; step++)
		{
			//1-8核同步工作控制
			if (ui_core_num == 0)
			{
				WaitAllCoreReady(s);
				if(step == 0)
				{
					int offset;
					if (DeviceId == 0)
					{
						tiling_input_0(s->io->input_image1, s->io->tiling_input, 0, 0, 0);
					}
					else if (DeviceId == 3)
					{
						offset = img_width[0] * (img_tiling[0][0][1] + img_tiling[0][1][1] + img_tiling[0][2][1] - 7);
						tiling_input_n(s->io->input_image1, s->io->tiling_input, offset, 0, 3);
					}
					else
					{
						offset = img_tiling[0][0][1] - 3;
						int idx = 1;
						while (idx < DeviceId)
						{
							offset += (img_tiling[0][DeviceId - 1][1] - 2);
							idx++;
						}
						tiling_input_1_n(s->io->input_image1, s->io->tiling_input, offset * img_width[0], 0, DeviceId);
					}
				}

				switch (step)
				{
					case 1:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 112, 28, 32/8);//后面四个数字需要每层核对 input ouput
						break;
					case 3:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 112, 28, 64/8);//后面四个数字需要每层核对
						break;
					case 5:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 56, 14, 128/8);//后面四个数字需要每层核对
						break;
					case 7:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 56, 14, 128/8);//后面四个数字需要每层核对
						break;
					case 9:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 28, 7, 256/8);//后面四个数字需要每层核对
						break;
					case 11:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 28, 7, 256/8);//后面四个数字需要每层核对
						break;
					case 12:
						for(int i = 0; i < 4; i++)
						{
							if(i == DeviceId)
							{
								tiling_to_big(s->io->tiling_input, s->io->output_img12, 12, DeviceId);
							}
							else
							{
								tiling_to_big(s->io->tobig_ddr_mem[i], s->io->output_img12, 12, i);
							}
						}
						break;
					default:
						break;
				}

				SetAllCoreStart(s);
#if CALC_TOTAL_TIME
				if (i_frame == 0 && step == 0)
				{
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
			}
			else
			{
				SetCoreReadyFlag(s);
				WaitCoreStartFlag(s);
			}
			//8核的工作
			if (step == 0)
			{
				LoadWeightBiasFromDDRToL2Channel_r1(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				conv_3x3_outputgroup_L2(s->io->tiling_input,s->weight->weight1,s->bias->bias1,s->io->tiling_output,0,s->ui_core, 2 ,0);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 1)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight2,s->bias->bias2,s->io->tiling_output,1,s->ui_core, 3,1,1,1);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 2)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight3,s->bias->bias3,s->io->tiling_input,2,s->ui_core, 1 ,2);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 3)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight4,s->bias->bias4,s->io->tiling_input,3,s->ui_core, 3,2,0 ,3);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 4)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight5,s->bias->bias5,s->io->tiling_output,4,s->ui_core, 1 , 4);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 5)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight6,s->bias->bias6,s->io->tiling_output,5,s->ui_core, 3,1,1,5);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 6)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight7,s->bias->bias7,s->io->tiling_input,6,s->ui_core, 1,6);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 7)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight8,s->bias->bias8,s->io->tiling_input,7,s->ui_core, 3,2,0,7);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 8)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight9,s->bias->bias9,s->io->tiling_output,8,s->ui_core, 1,8);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 9)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight10,s->bias->bias10,s->io->tiling_output,9,s->ui_core, 3,1,1,9);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 10)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight11,s->bias->bias11,s->io->tiling_input,10,s->ui_core, 1,10);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 11)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight12,s->bias->bias12,s->io->tiling_input,11,s->ui_core, 3,2,0,11);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 12)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img12,s->weight->weight13,s->bias->bias13,s->io->output_img13,12,s->ui_core, 1,12);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 13)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img13,s->weight->weight14,s->bias->bias14,s->io->output_img14,13,s->ui_core, 3,1,1,13);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 14)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img14,s->weight->weight15,s->bias->bias15,s->io->output_img15,13,s->ui_core, 1,14);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 15)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img15,s->weight->weight16,s->bias->bias16,s->io->output_img16,13,s->ui_core, 3,1,1,15);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if(step == 16)
			{
#if test_time
			long long StartTime = 0;
			long long EndTime = 0;
			if(s->ui_core == CORENUM)
			{
				TSCH = 0;
				TSCL = 0;
				StartTime = _itoll(TSCH, TSCL);
			}
#endif

			dw_conv_1x1_outputgroup_L2(s->io->output_img16,s->weight->weight17,s->bias->bias17,s->io->output_img17,13,s->ui_core, 1, 16);
#if test_time
			if(s->ui_core == CORENUM)
			{
				EndTime = _itoll(TSCH, TSCL);
				printf("banyuncore : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
			}
#endif
			}
			else if (step == 17)
			{
				LoadWeightBiasFromDDRToL2Channel_r2(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img17,s->weight->weight18,s->bias->bias18,s->io->output_img18,13,s->ui_core, 3,1,1,17);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 18)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img18,s->weight->weight19,s->bias->bias19,s->io->output_img19,13,s->ui_core, 1,18);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 19)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img19,s->weight->weight20,s->bias->bias20,s->io->output_img20,13,s->ui_core, 3,1,1,19);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 20)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img20,s->weight->weight21,s->bias->bias21,s->io->output_img21,13,s->ui_core, 1,20);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 21)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img21,s->weight->weight22,s->bias->bias22,s->io->output_img22,13,s->ui_core, 3,1,1,21);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 22)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img22,s->weight->weight23,s->bias->bias23,s->io->output_img23,13,s->ui_core, 1,22);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 23)
			{
				LoadWeightBiasFromDDRToL2Channel_r3(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img23,s->weight->weight24,s->bias->bias24,s->io->output_img24,14,s->ui_core, 3,2,0,23);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 24)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img24,s->weight->weight25,s->bias->bias25,s->io->output_img25,15,s->ui_core, 1,24);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 25)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->output_img25,s->weight->weight26,s->bias->bias26,s->io->output_img26,16,s->ui_core, 3,2,4,25);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 26)
			{
				LoadWeightBiasFromDDRToL2Channel_r4(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img26,s->weight->weight27,s->bias->bias27,s->io->output_img27,17,s->ui_core, 1,26);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 27)
			{
				LoadWeightBiasFromDDRToL2Channel_r5(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->output_img26,s->weight->weight28,s->bias->bias28,s->io->output_img28,18,s->ui_core, 1,27);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 28)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				globalavg_pool_group(s->io->output_img29,s->io->output_img30,19,s->ui_core);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}


			//各核step结束后执行的工作
			if (ui_core_num == 0)
			{
				while(IsAllCoreDone(s) == false)
				{
					asm("  nop 9");
				}



				//printf("core:%d, step:%d\n", ui_core_num, step);
//				if (step == 27)
//				{
//
//#if test_time
//				long long StartTime = 0;
//				long long EndTime = 0;
//				if(s->ui_core == CORENUM)
//				{
//					TSCH = 0;
//					TSCL = 0;
//					StartTime = _itoll(TSCH, TSCL);
//				}
//#endif
//				concat(s->io->output_img27,s->io->output_img28,s->io->output_img29);
//#if test_time
//				if(s->ui_core == CORENUM)
//				{
//					EndTime = _itoll(TSCH, TSCL);
//					printf("concat core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
//				}
//#endif
//				}
				if (step == 28)
				{

#if CALC_TOTAL_TIME
					EndTime = _itoll(TSCH, TSCL);
					printf("total time:%f ms\n", (EndTime - StartTime) / 1000000.0);
#endif
				}
				//SrioSendData(DeviceId, 1, (uint32_t)testData, 0xC0000000,4096);
				//SrioSendData(uint32_t LocalDeviceID, uint32_t DstDeviceId, uint32_t LocalAddr, uint32_t DstAddr, uint32_t DataLen);
				//, int rows, int weight, int height_1_n, int ch)

//				//分块策略：保持整行，对图像高度划分为7块，分别进行计算。
//													// con              con             con        10(5x2)                 conv
//				const int img_height[20]=     {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
//				const int img_width[20]=      {224,112,112,112,56 ,56 ,56 ,56 ,28 ,28 ,28 ,28 ,14 ,14 ,14 ,7   ,7   ,7   ,7   ,7   };
//				const int input_channel[20]=  {3  ,32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512 ,1024,1024,1024,1024};
//				const int output_channel[20]= {32 ,32 ,64 ,64 ,128,128,128,128,256,256,256,256,512,512,512,1024,1024,512 ,512 ,1024};
//                                              0   1   2   3   4   5  6   7    8   9   10  11  12  13  23  24   25   26  27    28
//
//
//				//分块参数说明：数组第1个参数表示图像宽度，第2个参数表示图像的高度，第3个参数表示图像Pading前实际分块位置。
//				const int img_tiling[12][4][3] =
//				{
//				    { {226, 58, 0}, {226, 58, 56}, {226, 58, 112}, { 226, 58, 168}},
//				    { {114, 30, 0}, {114, 30, 28}, {114, 30, 56}, { 114, 30, 84}},
//				    { {114, 30, 0}, {114, 30, 28}, {114, 30, 56}, { 114, 30, 84}},
//				    { {114, 30, 0}, {114, 30, 28}, {114, 30, 56}, { 114, 30, 84}},
//				    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
//				    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
//				    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
//				    { {58 , 16, 0}, {58 , 16, 14}, {58 , 16, 28}, { 58 , 16, 42}},
//				    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}},
//				    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}},
//				    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}},
//				    { {30 , 9 , 0}, {30 , 9 , 7}, {30 , 9 , 14}, { 30 , 9 , 21}}
//				};

				//ring
#if ring
				if((step <= 10 && step % 2 == 0))
				{
					switch (step)
					{
						case 0:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 32);//后面四个数字需要每层核对
							break;
						case 2:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 64);
							break;
						case 4:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 6:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 8:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						case 10:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						default:
							break;
					}

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

				}else if(step == 11){

					for(int i = 0;i < 3;i++)
					{
						if(i == 0)
							SrioSendData(DeviceId, (DeviceId + 1)%4, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[DeviceId],  output_channel[11]*img_height[12]*img_width[12]);
						else{
							SrioSendData(DeviceId, (DeviceId + 1)%4, (uint32_t)s->io->tobig_ddr_mem[((DeviceId + 4 - i) % 4)], (uint32_t)s->io->tobig_ddr_mem[((DeviceId + 4 - i) % 4)],  output_channel[11]*img_height[12]*img_width[12]);
						}
						while(1){
							//InvalidCache((void *)&RecvNum, 4);
							if(RecvNum != 0){
								break;
							}
							//printf("%d:,",RecvNum);
						}
						RecvNum -= 1;
					}
				}else if(step >= 12 && step <= 25 && step % 2 == 1)
				{
					//11层开始用通道划分 ////////////////////////////下面的按照通道传
					for(int i = 0;i < 3;i++)
					{
						switch (step)
						{
//						case 12:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img13 + ((DeviceId + 4 - i) % 4) * output_channel[12]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img13 + ((DeviceId + 4 - i) % 4) * output_channel[12]*img_height[13]*img_width[13] / 4, output_channel[12]*img_height[13]*img_width[13]);
//							break;
						case 13:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img14 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img14 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
//						case 14:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img15 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img15 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 15:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img16 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img16 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
//						case 16:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img17 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img17 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 17:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img18 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img18 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
//						case 18:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img19 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img19 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 19:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img20 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img20 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
//						case 20:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img21 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img21 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 21:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img22 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img22 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
//						case 22:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img23 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[14]*img_width[14] / 4, (uint32_t)s->io->output_img23 + ((DeviceId + 4 - i) % 4) * output_channel[13]*img_height[14]*img_width[14] / 4, output_channel[13]*img_height[14]*img_width[14]);
//							break;
						case 23:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img24 + ((DeviceId + 4 - i) % 4) * output_channel[14]*img_height[15]*img_width[15] / 4, (uint32_t)s->io->output_img24 + ((DeviceId + 4 - i) % 4) * output_channel[14]*img_height[15]*img_width[15] / 4, output_channel[14]*img_height[15]*img_width[15]);
							break;
//						case 24:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img25 + ((DeviceId + 4 - i) % 4) * output_channel[15]*img_height[16]*img_width[16] / 4, (uint32_t)s->io->output_img25 + ((DeviceId + 4 - i) % 4) * output_channel[15]*img_height[16]*img_width[16] / 4, output_channel[15]*img_height[16]*img_width[16]);
//							break;
						case 25:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img26 + ((DeviceId + 4 - i) % 4) * output_channel[16]*img_height[17]*img_width[17] / 4, (uint32_t)s->io->output_img26 + ((DeviceId + 4 - i) % 4) * output_channel[16]*img_height[17]*img_width[17] / 4, output_channel[16]*img_height[17]*img_width[17]);
							break;
							default:
								break;
						}
						while(1){
							//InvalidCache((void *)&RecvNum, 4);
							if(RecvNum != 0){
								break;
							}
							//printf("%d:,",RecvNum);
						}
						RecvNum -= 1;
					}
				}
#endif


//ps
#if ps

				if(step <= 10 && step % 2 == 0)
				{
					switch (step)
					{
						case 0:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 32);//后面四个数字需要每层核对
							break;
						case 2:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 64);
							break;
						case 4:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 6:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 8:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						case 10:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						default:
							break;
					}
				}else if(step == 11){

					if(DeviceId == 0)
					{
						SrioSendData(0, 1, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[0],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 2, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[0],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 3, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[0],  output_channel[11]*img_height[12]*img_width[12]);
					}else
					{
						SrioSendData(DeviceId, 0, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[DeviceId],  output_channel[11]*img_height[12]*img_width[12]);
					}

					if(DeviceId == 0)
					{
						while(1)
						{
							InvalidCache((void *)&RecvNum, 4);
							if(RecvNum == 3)
							{
								RecvNum -= 3;
								//printf("%d:; ",RecvNum);
								break;
							}
						}

						SrioSendData(0, 1, (uint32_t)s->io->tobig_ddr_mem[2], (uint32_t)s->io->tobig_ddr_mem[2],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 1, (uint32_t)s->io->tobig_ddr_mem[3], (uint32_t)s->io->tobig_ddr_mem[3],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 2, (uint32_t)s->io->tobig_ddr_mem[1], (uint32_t)s->io->tobig_ddr_mem[1],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 2, (uint32_t)s->io->tobig_ddr_mem[3], (uint32_t)s->io->tobig_ddr_mem[3],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 3, (uint32_t)s->io->tobig_ddr_mem[1], (uint32_t)s->io->tobig_ddr_mem[1],  output_channel[11]*img_height[12]*img_width[12]);
						SrioSendData(0, 3, (uint32_t)s->io->tobig_ddr_mem[2], (uint32_t)s->io->tobig_ddr_mem[2],  output_channel[11]*img_height[12]*img_width[12]);
					}else
					{
						while(1)
						{
							InvalidCache((void *)&RecvNum, 4);
							if(RecvNum == 3)
							{
								RecvNum -= 3;
								//printf("%d:; ",RecvNum);
								break;
							}
						}
					}

				}else if(step >= 12 && step <= 25 && step % 2 == 1){

					if(DeviceId != 0)
					{
						switch (step)
						{
//							case 12:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img13 + DeviceId * output_channel[12]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img13 + DeviceId * output_channel[12]*img_height[13]*img_width[13] / 4, output_channel[12]*img_height[13]*img_width[13]);
//								break;
							case 13:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img14 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img14 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
								break;
//							case 14:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img15 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img15 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//								break;
							case 15:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img16 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img16 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
								break;
//							case 16:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img17 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img17 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//								break;
							case 17:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img18 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img18 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
								break;
//							case 18:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img19 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img19 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//								break;
							case 19:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img20 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img20 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
								break;
//							case 20:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img21 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img21 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//								break;
							case 21:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img22 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img22 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
								break;
//							case 22:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img23 + DeviceId * output_channel[13]*img_height[14]*img_width[14] / 4, (uint32_t)s->io->output_img23 + DeviceId * output_channel[13]*img_height[14]*img_width[14] / 4, output_channel[13]*img_height[14]*img_width[14]);
//								break;
							case 23:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img24 + DeviceId * output_channel[14]*img_height[15]*img_width[15] / 4, (uint32_t)s->io->output_img24 + DeviceId * output_channel[14]*img_height[15]*img_width[15] / 4, output_channel[14]*img_height[15]*img_width[15]);
								break;
//							case 24:
//								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img25 + DeviceId * output_channel[15]*img_height[16]*img_width[16] / 4, (uint32_t)s->io->output_img25 + DeviceId * output_channel[15]*img_height[16]*img_width[16] / 4, output_channel[15]*img_height[16]*img_width[16]);
//								break;
							case 25:
								SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img26 + DeviceId * output_channel[16]*img_height[17]*img_width[17] / 4, (uint32_t)s->io->output_img26 + DeviceId * output_channel[16]*img_height[17]*img_width[17] / 4, output_channel[16]*img_height[17]*img_width[17]);
								break;
							default:
								break;
						}
						while(1)
						{
							InvalidCache((void *)&RecvNum, 4);
							if(RecvNum == 3)
							{
								RecvNum -= 3;
								//printf("%d:; ",RecvNum);
								break;
							}
						}
					}

					if(DeviceId == 0)
					{
						while(1)
						{
							InvalidCache((void *)&RecvNum, 4);
							if(RecvNum == 3)
							{
								RecvNum -= 3;
								//printf("%d:; ",RecvNum);
								break;
							}
						}

					for(int i = 1;i < 4;i++)
						for(int j = 0;j < 3;j++)
						{
							switch (step)
							{
//								case 12:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img13 + ((i+j+5) % 4) * output_channel[12]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img13 + ((i+j+5) % 4) * output_channel[12]*img_height[13]*img_width[13] / 4, output_channel[12]*img_height[13]*img_width[13]);
//									break;
								case 13:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img14 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img14 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
									break;
//								case 14:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img15 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img15 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//									break;
								case 15:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img16 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img16 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
									break;
//								case 16:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img17 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img17 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//									break;
								case 17:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img18 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img18 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
									break;
//								case 18:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img19 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img19 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//									break;
								case 19:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img20 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img20 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
									break;
//								case 20:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img21 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img21 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
//									break;
								case 21:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img22 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img22 + ((i+j+5) % 4) * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
									break;
//								case 22:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img23 + ((i+j+5) % 4) * output_channel[13]*img_height[14]*img_width[14] / 4, (uint32_t)s->io->output_img23 + ((i+j+5) % 4) * output_channel[13]*img_height[14]*img_width[14] / 4, output_channel[13]*img_height[14]*img_width[14]);
//									break;
								case 23:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img24 + ((i+j+5) % 4) * output_channel[14]*img_height[15]*img_width[15] / 4, (uint32_t)s->io->output_img24 + ((i+j+5) % 4) * output_channel[14]*img_height[15]*img_width[15] / 4, output_channel[14]*img_height[15]*img_width[15]);
									break;
//								case 24:
//									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img25 + ((i+j+5) % 4) * output_channel[15]*img_height[16]*img_width[16] / 4, (uint32_t)s->io->output_img25 + ((i+j+5) % 4) * output_channel[15]*img_height[16]*img_width[16] / 4, output_channel[15]*img_height[16]*img_width[16]);
//									break;
								case 25:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img26 + ((i+j+5) % 4) * output_channel[16]*img_height[17]*img_width[17] / 4, (uint32_t)s->io->output_img26 + ((i+j+5) % 4) * output_channel[16]*img_height[17]*img_width[17] / 4, output_channel[16]*img_height[17]*img_width[17]);
									break;
								default:
									break;
							}
						}
					}
				}
#endif

#if mesh
				if((step <= 10 && step % 2 == 0))
				{
					switch (step)
					{
						case 0:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 32);//后面四个数字需要每层核对
							break;
						case 2:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, 28, 64);
							break;
						case 4:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 6:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 14, 128);
							break;
						case 8:
							writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						case 10:
							writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 7, 256);
							break;
						default:
							break;
					}

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

				}else if(step == 11){

					for(int i = 0;i < 3;i++)
					{
						SrioSendData(DeviceId, (DeviceId + i + 1)%4, (uint32_t)s->io->tiling_input, (uint32_t)s->io->tobig_ddr_mem[DeviceId],  output_channel[11]*img_height[12]*img_width[12]);

						while(1){
							if(RecvNum != 0){
								break;
							}
						}
						RecvNum -= 1;
					}
				}else if(step >= 12 && step <= 25 && step % 2 == 1)
				{
					//11层开始用通道划分 ////////////////////////////下面的按照通道传
					for(int i = 0;i < 3;i++)
					{
						switch (step)
						{
						case 13:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img14 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img14 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
						case 15:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img16 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img16 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
						case 17:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img18 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img18 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
						case 19:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img20 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img20 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
						case 21:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img22 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, (uint32_t)s->io->output_img22 + DeviceId * output_channel[13]*img_height[13]*img_width[13] / 4, output_channel[13]*img_height[13]*img_width[13]);
							break;
						case 23:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img24 + DeviceId * output_channel[14]*img_height[15]*img_width[15] / 4, (uint32_t)s->io->output_img24 + DeviceId * output_channel[14]*img_height[15]*img_width[15] / 4, output_channel[14]*img_height[15]*img_width[15]);
							break;
						case 25:
							SrioSendData(DeviceId, (DeviceId + i + 1)%4,  (uint32_t)s->io->output_img26 + DeviceId * output_channel[16]*img_height[17]*img_width[17] / 4, (uint32_t)s->io->output_img26 + DeviceId * output_channel[16]*img_height[17]*img_width[17] / 4, output_channel[16]*img_height[17]*img_width[17]);
							break;
							default:
								break;
						}
						while(1){
							//InvalidCache((void *)&RecvNum, 4);
							if(RecvNum != 0){
								break;
							}
							//printf("%d:,",RecvNum);
						}
						RecvNum -= 1;
					}
				}
#endif



			}
			else
			{
				ResetCoreStartFlag(s);
			}
		}
	}
	while(1)
	{
		asm("  nop 9");
	}
}
