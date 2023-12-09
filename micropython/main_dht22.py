import dht
from time import sleep
d = dht.DHT22(machine.Pin(2))
while True:
  d.measure()
  temp = d.temperature()
  humi = d.humidity()
  print("temperature: {0}, humidity: {1}".format(temp, humi))
  sleep(2)
