import onnx
import numpy as np
import argparse
import time


start_time = time.time()
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument("--onnxname", type=str, default="0")
args = parser.parse_args()
model = onnx.load(args.onnxname)


record_rule=[
    'none',
    'conv_conv_1',
    'any_conv_x',
    'any_conv_pool',
    'matrix_transpose'
]
node_len = len(model.graph.node)
node=model.graph.node

def numb(out,n_len):
    countout=0
    for num in range(0,n_len):
        for i in range(0, len(model.graph.node[num].input)):
            if (out == model.graph.node[num].input[i]):
                countout=countout+1
    return countout



corder = np.zeros((node_len,), dtype=int)
op = np.zeros((50,), dtype=int)
dataflow=np.zeros((50,), dtype=int)

for num in range(0,4000*50):
    corder[1] = 1
    corder[2] = 1


for num in range(0,node_len-2):
    if((numb(model.graph.node[num].output[0],node_len)==1)and(numb(model.graph.node[num+1].output[0],node_len)==1)):
        sa=model.graph.node[num].output[0]
        sb=model.graph.node[num+1].output[0]
        sc=False
        sd=False
        for i in range(0,len(model.graph.node[num+1].input)):
            if(sa==model.graph.node[num+1].input[i]):
                sc=True
        for i in range(0, len(model.graph.node[num + 2].input)):
            if (sb == model.graph.node[num + 2].input[i]):
                sd = True
        if((sc==True)and(sd==True)):
            if ((model.graph.node[num].op_type == 'Conv') and (model.graph.node[num + 1].op_type == 'BatchNormalization')
                    and (model.graph.node[num + 2].op_type == 'LeakyRelu')):
                model.graph.node[num].op_type = 'CBR'
                se= model.graph.node[num+2].output[0]
                model.graph.node[num].output[0] =se
                corder[num + 1] = 1
                corder[num + 2] = 1
for num in range(0,node_len-1):
    if (numb(model.graph.node[num].output[0],node_len)==1):
        sa = model.graph.node[num].output[0]
        sc = False
        for i in range(0,len(model.graph.node[num+1].input)):
            if(sa==model.graph.node[num+1].input[i]):
                sc=True
        if (sc == True):
            if ((model.graph.node[num].op_type == 'Conv') and (model.graph.node[num + 1].op_type == 'BatchNormalization')):
                model.graph.node[num].op_type = 'CB'
                se = model.graph.node[num + 1].output[0]
                model.graph.node[num].output[0] = se
                corder[num + 1] = 1
            elif ((model.graph.node[num].op_type == 'Conv') and  (model.graph.node[num + 1].op_type == 'LeakyRelu')):
                model.graph.node[num].op_type = 'CR'
                se = model.graph.node[num + 1].output[0]
                model.graph.node[num].output[0] = se
                corder[num + 1] = 1


for i in range(node_len - 1, -1, -1):
    if(corder[i] == 1):
        model.graph.node.remove(model.graph.node[i])

node_len = len(model.graph.node)
corder_1 = np.zeros((node_len,), dtype=int)

for num in range(0,node_len-1):
    if (numb(model.graph.node[num].output[0],node_len)==1):
        sa = model.graph.node[num].output[0]
        sc = False
        for i in range(0, len(model.graph.node[num + 1].input)):
            if (sa == model.graph.node[num + 1].input[i]):
                sc = True
        if (sc == True):
            if (((model.graph.node[num].op_type == 'Conv') or (model.graph.node[num].op_type == 'CB') or
                 (model.graph.node[num].op_type == 'CR') or (model.graph.node[num].op_type == 'CBR'))
                    and (model.graph.node[num + 1].op_type == 'AveragePool')):
                model.graph.node[num].op_type = 'C_Pool'
                se = model.graph.node[num + 1].output[0]
                model.graph.node[num].output[0] = se
                corder_1[num + 1] = 1
            elif (((model.graph.node[num].op_type == 'Conv') or (model.graph.node[num].op_type == 'CB') or
                 (model.graph.node[num].op_type == 'CR') or (model.graph.node[num].op_type == 'CBR'))
                  and (model.graph.node[num+1].op_type == 'MaxPool')):
                model.graph.node[num].op_type = 'C_Pool'
                se = model.graph.node[num + 1].output[0]
                model.graph.node[num].output[0] = se
                corder_1[num + 1] = 1


for i in range(node_len - 1, -1, -1):
    if(corder_1[i] == 1):
        model.graph.node.remove(model.graph.node[i])

node_len = len(model.graph.node)
dataflow_record_1= np.zeros((node_len,), dtype=int)
dataflow_record_2= np.zeros((node_len,), dtype=int)
for num in range(0,node_len-1):
    if ((model.graph.node[num].op_type == 'DWConv')or(model.graph.node[num].op_type == 'Conv') or (model.graph.node[num].op_type == 'CB') or
            (model.graph.node[num].op_type == 'CR') or (model.graph.node[num].op_type == 'CBR') or (model.graph.node[num].op_type == 'C_Pool')):
        if ((model.graph.node[num+1].op_type == 'Conv') or (model.graph.node[num+1].op_type == 'CB') or
         (model.graph.node[num+1].op_type == 'CR') or (model.graph.node[num+1].op_type == 'CBR')):
            if (model.graph.node[num+1].attribute[2].ints[0]==1):
                if (numb(model.graph.node[num].output[0],node_len)==1):
                    sa = model.graph.node[num].output[0]
                    sc = False
                    for i in range(0, len(model.graph.node[num + 1].input)):
                        if (sa == model.graph.node[num + 1].input[i]):
                            sc = True
                    if (sc == True):
                        dataflow_record_1[num]=1
    else:
        if ((model.graph.node[num + 1].op_type == 'Conv') or (model.graph.node[num + 1].op_type == 'CB') or
                (model.graph.node[num + 1].op_type == 'CR') or (model.graph.node[num + 1].op_type == 'CBR')):
            if (model.graph.node[num + 1].attribute[2].ints[0] == 1):
                sa = model.graph.node[num].output[0]
                sc = False
                for i in range(0, len(model.graph.node[num + 1].input)):
                    if (sa == model.graph.node[num + 1].input[i]):
                        sc = True
                if (sc == True):
                    dataflow_record_2[num+1] = 1
    if ((model.graph.node[num].op_type == 'DWConv') or (model.graph.node[num].op_type == 'Conv') or (model.graph.node[num].op_type == 'CB') or
            (model.graph.node[num].op_type == 'CR') or (model.graph.node[num].op_type == 'CBR')):
        if (model.graph.node[num].attribute[2].ints[0]!=1):
            dataflow_record_2[num]=2
    elif (model.graph.node[num].op_type == 'C_Pool'):
        dataflow_record_2[num]=3
    elif (model.graph.node[num].op_type == 'MatMul') :
        dataflow_record_2[num]=4

node_len = len(model.graph.node)
input0=0
op = np.zeros((node_len,), dtype=int)
dataflow=np.zeros((node_len,), dtype=int)
for num in range(0,node_len-1):
    parm=0
    if (model.graph.node[num].op_type == 'DWConv'):
        op[num] = 0
        if (dataflow_record_1[num] == 1):
            dataflow[num] = 3
        else:
            dataflow[num] = 1
    elif (model.graph.node[num].op_type == 'Conv'):
        op[num] = 1
        if (dataflow_record_1[num]==1):
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 3
            else:
                dataflow[num] = 2
        else:
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 1
            else:
                dataflow[num] = 0
    elif (model.graph.node[num].op_type == 'CB'):
        op[num] = 2
        if (dataflow_record_1[num]==1):
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 3
            else:
                dataflow[num] = 2
        else:
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 1
            else:
                dataflow[num] = 0
    elif (model.graph.node[num].op_type == 'CR'):
        op[num] = 3
        if (dataflow_record_1[num]==1):
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 3
            else:
                dataflow[num] = 2
        else:
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 1
            else:
                dataflow[num] = 0
    elif (model.graph.node[num].op_type == 'CBR'):
        op[num] = 4
        if (dataflow_record_1[num]==1):
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 3
            else:
                dataflow[num] = 2
        else:
            if ((dataflow_record_2[num]==1) or(dataflow_record_2[num]==2)):
                dataflow[num] = 1
            else:
                dataflow[num] = 0
    elif (model.graph.node[num].op_type == 'C_Pool'):
        op[num] = 5
        if (dataflow_record_1[num]==1):
            dataflow[num] = 3
        else:
            dataflow[num] = 1
    elif (model.graph.node[num].op_type == 'MatMul'):
        op[num] = 6
        dataflow[num] = 4
    output0=0
    input0=output0
duration = time.time()-start_time
print(duration)

onnx.save(model,'newmodel.onnx')