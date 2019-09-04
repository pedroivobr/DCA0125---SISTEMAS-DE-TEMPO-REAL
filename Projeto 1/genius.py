import Adafruit_BBIO.GPIO as GPIO
import random as rand
import time

#arrays para a maquina e o jogador
genius_valores = 100*[0]
jogador_jogadas = 100*[0]

def iniciar():
	#definir valores do jogo
	for i in range(0,100):
		genius_valores[i] = rand.randint(0, 3)

def acender_led(i):
	GPIO.output(leds[i], 1)

def acender_leds():
	acender_led(0)
	acender_led(1)
	acender_led(2)
	acender_led(3)

def apagar_leds():
	GPIO.output(l0, 0)
	GPIO.output(l1, 0)
	GPIO.output(l2, 0)
	GPIO.output(l3, 0)

def apagar_led(i):
	GPIO.output(leds[i], 0)

def espera():
	apagar_leds()
	time.sleep(0.5)
	acender_led(0)
	acender_led(3)
	time.sleep(0.5)
	apagar_leds()
	time.sleep(0.25)
	acender_led(1)
	acender_led(2)
	time.sleep(0.5)
	apagar_leds()
	time.sleep(0.5)

def imprimir_valor(valor):
	GPIO.output(leds[0], valor[0])
	GPIO.output(leds[1], valor[1])
	GPIO.output(leds[2], valor[2])
	GPIO.output(leds[3], valor[3])

b0 = "P8_7"
b1 = "P8_8"
b2 = "P8_9"
b3 = "P8_10"
	
l0 = "P8_13"
l1 = "P8_14"
l2 = "P8_15"
l3 = "P8_16"
leds = [l0,l1,l2,l3]

GPIO.setup(b0, GPIO.IN)
GPIO.setup(l0, GPIO.OUT)

GPIO.setup(b1, GPIO.IN)
GPIO.setup(l1, GPIO.OUT)

GPIO.setup(b2, GPIO.IN)
GPIO.setup(l2, GPIO.OUT)

GPIO.setup(b3, GPIO.IN)
GPIO.setup(l3, GPIO.OUT)

iniciar()
rodada = 1
while True:
	#valor = [GPIO.input(b0), GPIO.input(b1) , GPIO.input(b2) , GPIO.input(b3)]
	
	for i in range(0,len(genius_valores)):
		print("Iniciou uma nova rodada")
		acender_leds()
		time.sleep(0.25)
		apagar_leds()
		time.sleep(0.25)
		acender_leds()
		time.sleep(1)
		#vez da maquina
		for j in range(0,rodada):
			apagar_leds()
			time.sleep(0.25)
			acender_led(genius_valores[j])
			time.sleep(1)
		#espera
		espera()
		#vez do jogador
		#j e o passo
		for j in range(0,rodada): 
			apagar_leds()
			#enquanto nao foi apertado algum botao
			valor = [0,0,0,0]
			while(sum(valor) == 0):
				valor = [GPIO.input(b0), GPIO.input(b1) , GPIO.input(b2) , GPIO.input(b3)]
				imprimir_valor(valor)
				time.sleep(0.2)
			time.sleep(1) #tempo de espera para cada teclada selecionada
			if valor[genius_valores[j]] == 0: #se o valor apertado for diferente do salvo no array da partida
				print("Perdeu :/")
				apagar_leds()
				acender_led(1)
				acender_led(3)
				time.sleep(1)
				#reseta os valores
				valor = [0,0,0,0]
				while(sum(valor) == 0):
					valor = [GPIO.input(b0), GPIO.input(b1) , GPIO.input(b2) , GPIO.input(b3)]
					time.sleep(0.1) #faca nada :v
				i = 0
				rodada = 0
				iniciar()
				break				
		rodada += 1
				