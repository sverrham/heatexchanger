import network
import socket
import time

from machine import Pin, ADC
import uasyncio as asyncio

from picozero import pico_temp_sensor

from umqttsimple import MQTTClient
import ubinascii
import json

import math

import SECRETS

led = Pin(15, Pin.OUT)
onboard = Pin("LED", Pin.OUT, value=0)

ssid = SECRETS.SSID
password = SECRETS.PASSWORD


html = """<!DOCTYPE html>
<html>
    <head> <title>Pico W</title> </head>
    <body> <h1>Pico W</h1>
        <p>%s</p>
    </body>
</html>
"""

def webpage(temp_pi, temp_in, temp_out, vent_mode):
    #Template HTML
    html = f"""
            <!DOCTYPE html>
            <html>
            <body>
            <form action="./balanced">
            <input type="submit" value="Balanced Ventilation" />
            </form>
            <form action="./in">
            <input type="submit" value="In Ventilation" />
            </form>
            <form action="./out">
            <input type="submit" value="Out Ventilation" />
            </form>
            <p>Temperature of pi cpu is {temp_pi}</p>
            <p>Temperature air in is {temp_in}</p>
            <p>Temperature air out is {temp_out}</p>
            <p>Ventilation is {vent_mode}</p>
            </body>
            </html>
            """
    return str(html)


wlan = network.WLAN(network.STA_IF)

def connect_to_network():
    wlan.active(True)
    wlan.config(pm = 0xa11140)  # Disable power-save mode
    wlan.connect(ssid, password)

    max_wait = 10
    while max_wait > 0:
        if wlan.status() < 0 or wlan.status() >= 3:
            break
        max_wait -= 1
        print('waiting for connection...')
        time.sleep(1)

    if wlan.status() != 3:
        raise RuntimeError('network connection failed')
    else:
        print('connected')
        status = wlan.ifconfig()
        print('ip = ' + status[0])

def get_temp(adc_pin):
    ntc_input = ADC(Pin(adc_pin))
    input_val = ntc_input.read_u16()
    voltage = input_val * 3.3/65535
    # Function to calculate tempearture from NTC resistor
    # In this function the NTC is 10kohm
    # The voltage is measured over the NTC resistor, so 10Kohm fixed resistor from VDD
    # Connected with NTC resistor to GND.
    # Taken from this example: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
    R1 = 10000
    #logR2, R2, T
    c1 = 1.009249522e-03
    c2 = 2.378405444e-04
    c3 = 2.019202697e-07
    R2 = R1 / ((3.3 / voltage) - 1.0)
    logR2 = math.log(R2)
    T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2))
    T = T - 273.15
  
    #print(f"adc input: {input_val} voltage: {voltage} temp: {T}")
    return T

def send_data_mqtt(mqtt_client, in_temp, out_temp, vent_mode):
    fan_id = "guest_room"
    topic_pub = b'hvac/balancedventilation/%s' %fan_id
    fan_data = {'mode': vent_mode,'temp_inside':in_temp, 'temp_outside':out_temp}
    #fan_data = {'direction':'in','temp_inside':12.11, 'temp_outside':22.34}
    data = json.dumps(fan_data)
    mqtt_client.publish(topic_pub, data)

def mqtt_data(mqtt_client):
    global pi_temp, in_temp, out_temp, vent_mode
    adc_pin_a = 26
    adc_pin_b = 27
    in_temp = get_temp(adc_pin_a)
    out_temp = get_temp(adc_pin_b)
    pi_temp = pico_temp_sensor.temp
    send_data_mqtt(mqtt_client, in_temp, out_temp, vent_mode)

pi_temp = 0
in_temp = 0
out_temp = 0
vent_mode = "balanced"
ventilation_mode = 0

async def serve_client(reader, writer):
    global pi_temp, in_temp, out_temp, vent_mode, ventilation_mode
    print("Client connected")
    request_line = await reader.readline()
    print("Request:", request_line)
    # We are not interested in HTTP request headers, skip them
    while await reader.readline() != b"\r\n":
        pass

    request = str(request_line)
    balanced_in = request.find('/balanced')
    vent_in = request.find('/in')
    vent_out = request.find('/out')
    
    print(balanced_in)
    if balanced_in == 6:
        vent_mode = "balanced"
        ventilation_mode = 0
        
    print(vent_in)
    if vent_in == 6:
        vent_mode = "in"
        ventilation_mode = 1
    
    print(vent_out)
    if vent_out == 6:
        vent_mode = "out"
        ventilation_mode = 2
        
    #response = html % stateis
    response = webpage(pi_temp, in_temp, out_temp, vent_mode)
    writer.write('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
    writer.write(response)

    await writer.drain()
    await writer.wait_closed()
    print("Client disconnected")

async def main():
    global ventilation_mode
    fan_a = Pin(14, Pin.OUT)
    fan_b = Pin(15, Pin.OUT)
    
    print('Connecting to Network...')
    connect_to_network()

    print('Setting up mqtt...')
    client_id = ubinascii.hexlify(machine.unique_id())
    mqtt_server = SECRETS.MQTT_SERVER
    mqtt_client = MQTTClient(client_id, mqtt_server)
    mqtt_client.connect()

    print('Setting up webserver...')
    asyncio.create_task(asyncio.start_server(serve_client, "0.0.0.0", 80))
    
    while True:
        onboard.on()
        print("heartbeat")
        
        if ventilation_mode == 0:
            fan_a.on()
            fan_b.off()
        elif ventilation_mode == 1:
            fan_a.on()
            fan_b.off()
        elif ventilation_mode == 2:
            fan_a.off()
            fan_b.on()
            
        for i in range(70):
            await asyncio.sleep(1)
            mqtt_data(mqtt_client)
            
        if ventilation_mode == 0:
            fan_a.off()
            fan_b.on()
        
        for i in range(70):
            await asyncio.sleep(1)
            mqtt_data(mqtt_client)
        
        
try:
    asyncio.run(main())
finally:
    asyncio.new_event_loop()

