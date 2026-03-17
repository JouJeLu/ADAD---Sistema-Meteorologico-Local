# ADAD - Estação Meteorológica Inteligente (IoT)

O ADAD é um projeto de sistemas embarcados desenvolvido para monitorar e analisar dados meterologicos em tempo real. Integrando hardware e algoritmos customizados, o sistema é capaz de prever condições climáticas críticas, como o ponto de geada, para auxiliar na tomada de decisão.

## Funcionalidades
- **Monitoramento em Tempo Real:** Temperatura, Umidade e Pressão Atmosférica.
- **Cálculos Avançados:** Cálculo automatizado do Ponto de Orvalho utilizando a Fórmula de Magnus.
- **Web Server Embarcado:** Visualização de dados diretamente do ESP32 através de um painel (dashboard) na rede local.
- **Comunicação I2C:** Comunicação otimizada entre os sensores e o microcontrolador.

## Tecnologias Utilizadas
- **Linguagem:** C++ (Framework Arduino)
- **Hardware:** Microcontrolador ESP32, Sensor BME280.
- **Protocolos:** I2C para sensores e Wi-Fi (HTTP) para o servidor web.

## Próximos Passos
- Implementar armazenamento de dados (Data Logging) em cartão SD.
- Integração com protocolo MQTT para monitoramento em nuvem.
- Evolução da interface visual do dashboard (Frontend)

## Fotos do Arduino e o Sensor na Protoboard
#Pojeto na protoboard
![20260316_200116](https://github.com/user-attachments/assets/f5de56c3-e1d7-47ff-a7a6-bc7dd0c8bb48)
#Sensor BME 280
![20260316_200129](https://github.com/user-attachments/assets/c5b15d9f-41f5-42cd-b811-bd5ffa2c06ea)
#ESP 32
![20260316_200124](https://github.com/user-attachments/assets/a134d12d-ecef-471d-a204-83a6d8164a8f)

## Esquema das conexões
<img width="560" height="362" alt="image" src="https://github.com/user-attachments/assets/98473473-1644-40ab-b314-9a5c2ec5451c" />
