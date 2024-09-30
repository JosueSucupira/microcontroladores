## Prática 7
Controlar a razão cíclica dos canais de PWM conectados a um LED RGB via USART. O STM32 deve ser capaz de reconhecer os seguintes comandos:

1) rXX\n ou RXX\n, bXX\n ou BXX\n, gXX\n ou GXX\n - Modifica para o valor indicado em XX {00 a 99} o valor da razão cíclica do PWM que controla os LEDs Vermelho (r ou R), Azul (b ou B) ou Verde (g ou G). Note a presença do caractere '\n' para finalizar o comando. 

2) d\n ou D\n - Entra ou sai {TOOGLE} do modo de demonstração, equivalente ao comportamento descrito na prática 05.

OBS: durante o modo de demonstração, o STM32 deve ignorar os outros comandos