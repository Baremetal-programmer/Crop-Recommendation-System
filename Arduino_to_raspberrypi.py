import serial
import requests
import time
api_url="https://api.thingspeak.com/update"
api_key=""
ser=serial.serial('/dev/ttyACM0',4000,timeout=1)
try:
    while True:
        data=ser.readline().decode('utf-8').rstrip()
        print("Received data from Arduino",data)
        readings = data.split(',')
        if len(readings) >=6 :
            N,P,K,pH,Moisture,Temp = readings
            data_to_send = {'api_key': api_key,'field1': N,'field2': P,'field3': K,'field4': pH,'field5': Moisture,'field6':Temp}
            response = requests.post(api_url, params = data_to_send)
            if response.status_code == 200:
                print("Data sent successfully")
            else:
                print(f"Failed to send data to thingspeak. Status Code:{response.status_code}")
                print(response.text)
            time.sleep(5)    
        else:
            print("Invalid data")
            time.sleep(5)
except KeyboardInterrupt:
    ser.close()
    print("Serial communication end.")
            