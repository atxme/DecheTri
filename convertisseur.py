from keras import backend as K
from keras.models import load_model
from tensorflow.python.framework import graph_util
from tensorflow.python.framework import graph_io


weight_file_path = '/home/christophe/Documents/code/waste_classification_net-2.2.0.h5'
net_model = load_model(weight_file_path)
sess = K.get_session()

output_node_names = [out.op.name for out in net_model.outputs]
constant_graph = graph_util.convert_variables_to_constants(sess, sess.graph.as_graph_def(), output_node_names)
graph_io.write_graph(constant_graph, '/home/christophe/Documents/model/', 'net.pb', as_text=False)
#print('saved the constant graph (ready for inference) at: ', osp.join('output_folder_path', 'output.pb'))
print("done ")