import cv2 as cv 
import tensorflow as tf 
from keras.models import load_model
import pathlib  
import warnings
from videostream import stream
import time 


warnings.simplefilter(action='ignore', category=FutureWarning)


net=pathlib.Path('/home/christophe/Documents/code/projet dechet/neural_network.2.0.h5')


    
def predict(frame):
    model=load_model(net)
    #frame=np.resize(frame,(225,225,3))


def main_test():
    cap=cv.cuda.VideoCapture(2)
    start_time=0
    frame_count=0
    while 1:
        
        if cap.isOpened():
            
            ret,frame=cap.read()
            cv.cuda.cvtColor(frame, cv.COLOR_BGR2RGB, dst=frame)
            
            
            frame_count+=1
            
            cv.imshow('flux video',frame)
            
            elapsed_time = time.time() - start_time
            fps = frame_count / elapsed_time
            print(fps)
        else :
            print("error camera")
            
        if cv.waitKey(1) == ord('q'):
                break
        

main_test()



