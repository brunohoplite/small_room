#!/usr/bin/env python3
import threading
from flask import Flask, render_template, request
from control_box import ControlBox


# Function to run Flask app
def run_flask(app):
    app.run(host='0.0.0.0', port=5000)

if __name__ == '__main__':
    box = ControlBox(detectPin=23, brokerIp="127.0.0.1")

    # Flask app
    app = Flask(__name__)

    @app.route('/')
    def index():
        return render_template('index.html')

    @app.route('/update', methods=['POST'])
    def update():
        global brightness, mode
        brightness = int(request.form['brightness'])
        mode = request.form['mode']
        print(f'Received mode: {mode}, brightness: {brightness}')
        box.set_mode(mode)
        box.processDimmerCommand(brightness)

        return "OK", 200

    # Start Flask in a separate thread
    flask_thread = threading.Thread(target=run_flask, args=[app], daemon=True)
    flask_thread.start()

    while(True):
        box.run()
