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

