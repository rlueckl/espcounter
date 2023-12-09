# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import os, machine
#os.dupterm(None, 1) # disable REPL on UART(0)
import gc
#import webrepl
#webrepl.start()
gc.collect()


def do_connect():
  import network
  f = open('wifi.txt', 'r')
  data = f.read()
  ssid = data.split(';')[0]
  pswd = data.split(';')[1]
  #print('ssid: {0}, pass: {1}'.format(ssid, pswd))
  sta_if = network.WLAN(network.STA_IF)
  if not sta_if.isconnected():
    print('connecting to network...')
    sta_if.active(True)
    sta_if.connect(ssid, pswd)
    while not sta_if.isconnected():
      pass
  print('network config:', sta_if.ifconfig())
do_connect()
