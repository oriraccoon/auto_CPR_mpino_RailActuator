import cv2 as cv
import numpy as np


set_color = True

# 빨간색 범위 설정
saturation_th = 30
value_th = 30
color = 0
ranges = 20

lower_red1 = np.array([color - ranges + 180, saturation_th, value_th])
upper_red1 = np.array([180, 255, 255])
lower_red2 = np.array([0, saturation_th, value_th])
upper_red2 = np.array([color, 255, 255])
lower_red3 = np.array([color, saturation_th, value_th])
upper_red3 = np.array([color + ranges, 255, 255])

cv.namedWindow('img_color')
cv.namedWindow('img_result')

cap = cv.VideoCapture(0)

while(True):
    ret, img_color = cap.read()
    img_color = cv.flip(img_color, 1)

    if ret == False:
        continue

    img_color2 = img_color.copy()
    img_hsv = cv.cvtColor(img_color2, cv.COLOR_BGR2HSV)

    height, width = img_color.shape[:2]
    cx = int(width / 2)
    cy = int(height / 2)

    rectangle_color = (0, 255, 0)
    cv.rectangle(img_color, (cx - 30, cy - 30), (cx + 30, cy + 30), rectangle_color, 5)

    if set_color:
        img_mask1 = cv.inRange(img_hsv, lower_red1, upper_red1)
        img_mask2 = cv.inRange(img_hsv, lower_red2, upper_red2)
        img_mask3 = cv.inRange(img_hsv, lower_red3, upper_red3)
        temp = cv.bitwise_or(img_mask1, img_mask2)
        img_mask = cv.bitwise_or(img_mask3, temp)

        kernel = np.ones((11,11), np.uint8)
        img_mask = cv.morphologyEx(img_mask, cv.MORPH_OPEN, kernel)
        img_mask = cv.morphologyEx(img_mask, cv.MORPH_CLOSE, kernel)

        img_result = cv.bitwise_and(img_color, img_color, mask=img_mask)

        numOfLabels, img_label, stats, centroids = cv.connectedComponentsWithStats(img_mask)
        for idx, centroid in enumerate(centroids):
            if stats[idx][0] == 0 and stats[idx][1] == 0:
                continue
            if np.any(np.isnan(centroid)):
                continue
            x, y, width, height, area = stats[idx]
            centerX, centerY = int(centroid[0]), int(centroid[1])
            if area > 1500:
                cv.circle(img_color, (centerX, centerY), 10, (0, 0, 255), 10)
                cv.rectangle(img_color, (x, y), (x + width, y + height), (0, 0, 255))
               
                # 중심 사각형 안에서 색깔이 검출될 때 "detect" 출력
                if (cx - 30) < centerX < (cx + 60) and (cy - 60) < centerY < (cy + 60):
                    print("detect")
                   

        cv.imshow('img_result', img_result)

    cv.imshow('img_color', img_color)

    key = cv.waitKey(1) & 0xFF

    if key == 27: # esc
        break

cap.release()
cv.destroyAllWindows()
