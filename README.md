# Arduino RFID & Servo


## Komponen:

- Arduino Uno
- BreadBoard
- Motor Servo
- RFID RC522
- Kabel Jumper Male & Female

## Wiring
- RFID RC522: 
	- 3.3 -> 3.3V
	- GND -> GND
	- RST -> PIN 9
	- SDA -> PIN 10
	- MOSI -> PIN 11
	- MISO -> PIN 12
	- SCK -> PIN 13
- Servo:
	- GND -> GND
	- POWER -> 5V
	- SIGNAL -> PIN 6


## Algoritma:
	- Kartu dapat didaftarkan
	- Kartu dapat dicek terdaftar atau tidak
	- Jika kartu terdaftar, maka akan memutar servo
	- Jika kartu tidak terdaftar tidak akan memutar servo
	- Jika ingin mendaftarkan kartu, ketikkan register di prompt serial monitor, lalu scan kartu
	- Jika ingin mereset pengecekan kartu, ketikkan cek di prompt serial monitor


## Usable Pin:
	- Pin Servo : 6
	- Pin Slave Select RFID: 10
	- Pin Reset RFID: 9