# Sprint4-IoT-IoB-Generative-IA-TechLab
Apresentação do projeto, código-fonte, estrutura e README, para atendimento da Sprint 4 da disciplina de IoT, IoB & Generative IA.

## Tecnologias utilizadas na solução (Todas as disciplinas)
Solução em Cloud, com aplicação Web Java (MVC), banco de dados Postgres, banco de dados Oracle, API em .NET, APP em React-Native, atuadores RFID simulados em ESP32, gestão do fluxo CI/CD com Azure Devops e planejamento de release e testes gerenciados via Microsoft Azure.

## Instruções para execução em produção (Protótipo Simulado)
🧠 Visão Geral da Solução
O sistema realiza triangulação por RSSI para estimar a posição (X, Y) de objetos (tags) em um espaço físico.

- ESP32 Issuers (3 unidades): transmitem sinais Wi-Fi/BLE que serão detectados.
- ESP32 Server (1 unidade): recebe o sinal RSSI de cada issuer, converte em distância e calcula a posição.
- Página HTML: exibe graficamente a localização dos objetos.

⚙️ Requisitos de Instalação
🔸 Hardware
- 4 placas ESP32 DevKit V1
- Cabo USB tipo A para micro-USB
- Roteador Wi-Fi local para comunicação entre os dispositivos

🔸 Software
- Arduino IDE (versão 2.0 ou superior)
- Biblioteca ESP32 instalada:
- Em Arduino IDE → Ferramentas → Placa → Gerenciador de Placas
Bibliotecas adicionais (caso não estejam instaladas):
- WiFi.h
- WebServer.h
- ArduinoJson.h (para comunicação via JSON entre ESP32 e HTML)

Navegador Web (Google Chrome ou Edge)

- Configurar a Rede Wi-Fi
Em cada sketch, edite as credenciais:<br>
const char* ssid = "NOME_DA_REDE";<br>
const char* password = "SENHA_DA_REDE";
* Todos os ESP32 devem estar conectados à mesma rede local.

- Executar o Server e Acessar a Página HTML.
