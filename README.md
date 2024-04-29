## Xenos : Dataflow-Centric Optimization to Accelerate Model Inference on Edge Devices
Xenos is the first inference framework focusing on dataflow-centric optimization whereas the other inference frameworks mainly adopt operator-centric optimization. It accelerates edge-based devices inference through dataflow optimization and flexible model partition.
## Paper and Presentation
Xenos : Dataflow-Centric Optimization to  Accelerate Model Inference on Edge Devices(DASFAA'23)

Towards A High-Performance Dataflow-Centric Framework for Edge-Based Inference(Major Revision)

If you find our work helpful to your research or project, we would very appreciate it if you could add a star to our repo and/or cite our papers. The bibs for the papers are as below.
```
@inproceedings{zhang2023xenos,
  title={Xenos: Dataflow-Centric Optimization to Accelerate Model Inference on Edge Devices},
  author={Zhang, Runhua and Jiang, Hongxu and Tian, Fangzheng and Geng, Jinkun and Li, Xiaobin and Ma, Yuhang and Zhu, Chenhui and Dong, Dong and Li, Xin and Wang, Haojie},
  booktitle={International Conference on Database Systems for Advanced Applications},
  pages={535--545},
  year={2023},
  organization={Springer}
}
```
## Clone Project
```
git clone https://github.com/Happyrhzhang/Xenos.git
```

## File Structure
The core part includes two modules.
- Vertical: Operator Linking
- Hardware Test : On-board testing

## Install & Dependencies
- Windows 10
- Compiler: CCS 6.1.0, VS 2019, PyCharm 2021
- Python 3.9
- Pytorch 1.12
- ONNX 1.10.2
- XGBoost 1.7.4
- Sklearn 1.2.1 
- PDK: C6678 1.1.2.6
- Xdctools 3.25.3.72

## Test Example

### Test1 : Operator Linking Optimization
- S1: On the PC Compiler (PyCharm), compile and run:
```
python ONNXtoIR.py
```
or terminal, run:
```
python ONNXtoIR.py --onnxname "name.onnx"
```
- S2: On the PC Compiler (VS), compile and run:
```
./FlexPie/src/dp
```
### Test2 : D-Xenos on-board testing 
- S1: Load Project
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-1.jpg" width="400" height="200">
</div>

- S2: To add a device, follow the following process to add a device. One to four can be added.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-2.jpg" width="400" height="200">
</div>

- S3: Select the corresponding topology, such as Ring, PS or Mesh.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-3.jpg" width="400" height="200">
</div>

- S4:Compile the executable separately for device.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-4.jpg" width="400" height="200">
</div>

- S5:Load to board and run.

### Test3 : D-Xenos on-board testing 
- S1: Load Project
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-1.jpg" width="400" height="200">
</div>

- S2: To add a device, follow the following process to add a device. One to four can be added.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-2.jpg" width="400" height="200">
</div>

- S3: Select the corresponding topology, such as Ring, PS or Mesh.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-3.jpg" width="400" height="200">
</div>

- S4:Compile the executable separately for each device.
<div align="left">
  <img src="https://github.com/Happyrhzhang/FlexPie/blob/main/Fig/2-4.jpg" width="400" height="200">
</div>

- S5:Load to board and run.

## Future Plan
- Support deployment applications of larger scale nodes.
- Extending Xenos to more edge hardware.
