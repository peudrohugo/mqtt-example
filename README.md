# MQTT Communication with ESP32

## Equipe

- **Douglas Vieira**
- **Pedro Hugo**

Este projeto tem como objetivo implementar uma aplicação ESP32 utilizando o protocolo MQTT para comunicação com um broker hospedado no HiveMQ Cloud. Assim como gerenciar o estado de um LED e monitorar o estado de um botão físico.

## Funcionalidades

1. **Estado do Botão**:

   - O ESP32 monitora o estado de um botão conectado a placa e publica seu estado (Pressionado ou não) no tópico `/esp32/button_output`. Vale ressaltar que o botão não aciona o LED da própria placa, pois seu funcionamento simula o acionamento do LED em outra placa que estaria escrita ao tópico citado anteriormente.

2. **Estado do LED**:

   - O ESP32 é inscrito no tópico `esp32/led_input` e aguarda mudança de estado pelo Client MQTT (simulando um segundo ESP32) controlando o estado do LED conectado. No caso do tópico atrelado ao LED, ele simula o funcionamento da segunda placa que publicaria o estado do LED da primeira placa.

## Config Broker MQTT

- **Broker**: HiveMQ Cloud
- **Tópico para Publicação**: `/esp32/button_output`
- **Tópico para Assinatura**: `/esp32/led_input`
- **Usuário**: `pedrohugo`
- **Senha**: `Pe12345678`
- **Protocolo**: MQTT 3.1.1

## Config dos Componentes

- **Botão**: GPIO 33.
- **LED**: GPIO 26.

## Como Executar

1. Configure o broker MQTT no HiveMQ Cloud, assim como seu Web Client para gerenciamento dos tópicos.
2. Faça o Flash no ESP32.
3. Conecte o ESP32 à rede Wi-Fi configurada.
4. Use o Client MQTT para enviar mensagens ao tópico `esp32/led_input` e controlar o LED.
5. Monitore o estado do botão no tópico `esp32/button_output` utilizando o mesmo Client MQTT.
