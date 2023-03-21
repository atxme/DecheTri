import cv2 as cv
import numpy as np
import sys 



class stream :
    
    def __init__(self) -> None:
        self.camera=cv.VideoCapture(0)
        self.size=(225,225)
        
    def stop(self):
        self.camera.release()
        cv.destroyAllWindows()
        
    def flux(self):
        if not self.camera.isOpened():
            print("No camera")
            self.stop()
            sys.exit(0)
        else :
            temp=self.camera.read()
            image=np.array(temp)
            frame=cv.resize(image,(225,225))
            
            cv.waitKey(0)
            return frame
        
    