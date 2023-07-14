# importamos librerias necesarias
import cv2
import mediapipe as mp
from requests import post
import numpy as np
from time import sleep
from flask import Flask, request, jsonify
from flask_cors import CORS


# Flask

app = Flask("Mecha-Galicia")
app.config["SECRET_KEY"] = "secret!"
app.config["CORS_HEADERS"] = "Content-Type"
CORS(app)

# URL to ESP32
ESP_URI = "http://192.168.137.32/move"

# Control http requests
direction_change = True
actual_direction = "STOP"


# arreglo para guardar indices
cx = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]


# Se carga el objeto de manos de mediapipe
mpHands = mp.solutions.hands
hands = mpHands.Hands(False, 2)


@app.route("/process-frame", methods=["POST"])
def process_frame():
    global direction_change, actual_direction, cx, mpHands, hands
    image_data = request.files["image"].read()
    nparr = np.frombuffer(image_data, np.uint8)
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:  # recorremos todos los puntos
            # obtaining information
            # Guardamos los puntos de la mano en variables nuevas para usarlas después
            for id, lm in enumerate(handLms.landmark):
                h, w, c = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                # print(id, cx, cy)
                if id == 4:  # Punta del pulgar
                    cx4 = cx
                    cy4 = cy
                if id == 0:  # Palma
                    cx0 = cx
                    cy0 = cy
                if id == 12:  # Punta del dedo medio
                    cx12 = cx
                    cy12 = cy
                if id == 16:  # Punta del dedo anular
                    cx16 = cx
                    cy16 = cy
                if id == 8:  # Punta del indice
                    cx8 = cx
                    cy8 = cy
                if id == 20:  # Punta del menique
                    cx20 = cx
                    cy20 = cy
        if cy12 > cy0:
            diff = cy12 - cy0
            if diff > 0 and diff < 100:
                if actual_direction != "SMALL FORWARD":
                    actual_direction = "SMALL FORWARD"
                    direction_change = True
            if diff > 100 and diff < 150:
                if actual_direction != "FORWARD":
                    actual_direction = "FORWARD"
                    direction_change = True

            if diff > 150 and diff < 200:
                if actual_direction != "TURBO FORWARD":
                    actual_direction = "TURBO FORWARD"
                    direction_change = True
        else:
            if cy4 > cy0:
                if actual_direction != "LEFT":
                    actual_direction = "LEFT"
                    direction_change = True
            elif cy20 > cy0:
                if actual_direction != "RIGHT":
                    actual_direction = "RIGHT"
                    direction_change = True
            else:
                if actual_direction != "STOP":
                    actual_direction = "STOP"
                    direction_change = True
    if direction_change:
        # direction_change = False
        # print("Happy! Direction", actual_direction, "🥰")
        # return jsonify({"message": actual_direction}), 200
        direction_change = False
        try:
            response = post(ESP_URI, {"direction": actual_direction})
            if response.ok:
                print("Happy! Direction", actual_direction, "🥰")
                return jsonify({"message": actual_direction}), 200
            else:
                print("Panic! Direction", actual_direction, "😣")
                return jsonify({"message": "Failed to send to robot"}), 400
        except:
            print("Panic! Direction", actual_direction, "😣")
            return jsonify({"message": "Server failed"}), 500

    return jsonify({"message": "COOL"}), 200


@app.route("/manual", methods=["POST"])
def handleManual():
    direaction = request.form.get("direction")
    if direaction:
        try:
            response = post(ESP_URI, {"direction": direaction})
            if response.ok:
                print("Happy! Direction", direaction, "🥰")
                return jsonify({"message": direaction}), 200
            else:
                print("Panic! Direction", direaction, "😣")
                return jsonify({"message": "Failed to send to robot"}), 400
        except:
            print("Panic! Direction", direaction, "😣")
            return jsonify({"message": "Server failed"}), 500
    return jsonify({"message": "Bad request"}), 400


if __name__ == '__main__':
    app.run(port=8084, debug=True)
