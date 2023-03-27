import cv2 as cv 
import pathlib 
import serial
import numpy as np 
from threading import Thread
import os 
import sys 
import time 

class_names=["can","plastic"]

cv.cuda.setDevice(2)  #set gpu to use device 

#check if file exist 
check=os.path.exists("/home/christophe/Documents/code/projet dechet/torchNet_test4.onnx")
if check==True:
    model_path=str("/home/christophe/Documents/code/projet dechet/torchNet_test4.onnx")
else :
    sys.exit(0)

def data_transfert(data):
    ser=serial.Serial('/dev/ttyUSB0',9600)
    ser.write(data)
    ser.close()

def main():
    #read onnx model 
    model = cv.dnn.readNetFromONNX(model_path)
    model.setPreferableBackend(cv.dnn.DNN_BACKEND_CUDA)
    model.setPreferableTarget(cv.dnn.DNN_TARGET_CUDA)
    
    cap=cv.VideoCapture(2)
    if not cap.isOpened():
        print("Camera error")
    else:
        while True:
            ret, frame = cap.read()
            
            if not ret:
                print("Erreur de lecture d'image ")
                break
            
            blob = cv.dnn.blobFromImage(frame, 1/255, (224, 224), (0,0,0), swapRB=True)
            model.setInput(blob)
            outputs = model.forward()
            
            # convertir l'image en un objet GpuMat pour pouvoir l'afficher sur GPU
            gpu_frame = cv.cuda_GpuMat()
            gpu_frame.upload(frame)

            
            class_idx = np.argmax(outputs)
            print("la classe est :",class_names[class_idx])
            print(outputs)
            data_transfert(class_idx)
            
            final_frame=gpu_frame.download()
            cv.imshow('frame', final_frame)
            
            if cv.waitKey(1) == ord('q'):
                break
        
        cap.release()
        cv.destroyAllWindows()
        
main()

   
        






    
    