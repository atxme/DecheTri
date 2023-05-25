import cv2

# capture video from webcam and save each frame as a jpg
cap = cv2.VideoCapture(1)

i = 0
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    # Display the resulting frame
    cv2.imshow('frame',frame)
    # Save the resulting frame
    cv2.imwrite('photos/frame'+str(i)+'.jpg',frame)
    i += 1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
