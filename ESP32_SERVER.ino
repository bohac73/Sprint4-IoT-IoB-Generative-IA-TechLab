#include <WiFi.h>
#include <WebServer.h>
#include <math.h>

const char* ssid = "meu usuário";
const char* password = "minha senha";

WebServer server(80);

struct Node {
  String id;
  int rssi;
};

Node nodes[3];
int nodeCount = 0;

String pageTemplate() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>SGP Mottu</title>";
  html += "<style>";
  html += "body {margin:0; font-family: 'Segoe UI', Tahoma; text-align:center; ";
  html += "background: linear-gradient(135deg,#0f2027,#203a43,#2c5364); color:#00e5ff;}";
  html += "h1 {color:#00e5ff; text-shadow:0 0 10px #00e5ff,0 0 20px #00e5ff;}";
  html += "canvas {border:2px solid #00e5ff; border-radius:10px; background:rgba(0,0,0,0.6); box-shadow:0 0 15px #00e5ff;}";
  html += "footer {margin-top:20px; font-size:12px; color:#aaa;}";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Sistema de Gestão de Pátios (Coordenadas)</h1>";
  html += "<canvas id='mapa' width='500' height='400'></canvas>";
  // bloco de informações lateral esquerda
  html += "<div style='position:absolute; left:20px; top:100px; text-align:left; color:#FFFFFF; font-family:Segoe UI; font-size:14px; line-height:1.5;'>";

  html += "<strong>MOTTU LOCAÇÃO DE VEÍCULOS LTDA</strong><br>";
  html += "Data: <span id='dataAtual'></span><br><br>";

  html += "<strong>Unidade operacional:</strong><br>";
  html += "003 - Mottu Lapa<br>";
  html += "Rua Catão, 1343 – Vila Romana<br>";
  html += "São Paulo – SP<br><br>";

  html += "<strong>Total de motos em serviço:</strong> 42<br>";

  html += "</div>";

  html += "<footer>Powered by TechLab&reg; 2025</footer>";
  html += "<script>";
  html += "var ctx=document.getElementById('mapa').getContext('2d');";
  html += "ctx.clearRect(0,0,500,400);";

  // --- Divisão do pátio: rampa, box rápido, bancada motor ---

  // linha suave para setores
  html += "ctx.strokeStyle='rgba(0, 229, 255, 0.25)';";
  html += "ctx.lineWidth=2;";

  // linhas verticais
  html += "ctx.beginPath(); ctx.moveTo(300,0); ctx.lineTo(300,400); ctx.stroke();";  // 70%
  html += "ctx.beginPath(); ctx.moveTo(400,0); ctx.lineTo(400,400); ctx.stroke();";  // 70% + 10%

  // textos pequenos em cada seção
  html += "ctx.fillStyle='rgba(0, 229, 255, 0.60)';";
  html += "ctx.font='12px Segoe UI';";

  html += "ctx.fillText('Rampa de manutenção', 10, 15);";
  html += "ctx.fillText('Box rápido', 305, 15);";
  html += "ctx.fillText('Bancada motor', 405, 15);";

  for (int i = 0; i < nodeCount; i++) {
    int rssi = nodes[i].rssi;

    // calcula distância estimada (metros)
    float distancia = pow(10, ((-40 - rssi) / (10 * 2.0)));

    // ângulo base (120 graus entre cada issuer)
    float angle = i * (120 * PI / 180.0);

    // coordenadas estimadas
    float x = distancia * cos(angle);
    float y = distancia * sin(angle);

    // origem (0,0) no centro do pátio (500x400)
    const int CANVAS_W = 500, CANVAS_H = 400;
    const int CX = CANVAS_W / 2;  // 250
    const int CY = CANVAS_H / 2;  // 200
    const float SCALE = 20.0;     // mesmo fator que você já usava

    int posX = CX + (int)(x * SCALE);
    int posY = CY - (int)(y * SCALE);  // OBS: eixo Y do canvas é invertido

    // -------- C E R C A  centralizada (mantém moto e texto visíveis) ----------
    const int PAD_LEFT = 50;    // margem esquerda
    const int PAD_RIGHT = 10;   // margem direita
    const int PAD_TOP = 30;     // margem superior
    const int PAD_BOTTOM = 45;  // margem inferior (texto e coordenadas)

    if (posX < PAD_LEFT) posX = PAD_LEFT;
    if (posX > CANVAS_W - PAD_RIGHT) posX = CANVAS_W - PAD_RIGHT;
    if (posY < PAD_TOP) posY = PAD_TOP;
    if (posY > CANVAS_H - PAD_BOTTOM) posY = CANVAS_H - PAD_BOTTOM;
    // -------------------------------------------------------------------------

    // raio do círculo
    int raio = 3;  // ponto fixo pequeno

    // estilo
    html += "ctx.strokeStyle='#00e5ff';";
    html += "ctx.fillStyle='#FFFFFF';";
    html += "ctx.font='10px Segoe UI';";

    // cor padrão azul neon para todos os elementos
    html += "ctx.fillStyle='#00e5ff';";

    // Posição do ponto
    html += "ctx.fillText('🏍️'," + String(posX - 5) + "," + String(posY - 5) + ");";

    // Texto do ID logo abaixo do ponto
    html += "ctx.fillText('" + nodes[i].id + "'," + String(posX - 40) + "," + String(posY + 5) + ");";

    // Texto das coordenadas abaixo do ID
    html += "ctx.fillText('X=" + String(x, 2) + "m Y=" + String(y, 2) + "m'," + String(posX - 40) + "," + String(posY + 18) + ");";
  }

  html += "var hoje = new Date();";
  html += "var dia = String(hoje.getDate()).padStart(2, '0');";
  html += "var mes = String(hoje.getMonth() + 1).padStart(2, '0');";
  html += "var ano = hoje.getFullYear();";
  html += "document.getElementById('dataAtual').innerHTML = dia + '/' + mes + '/' + ano;";

  html += "</script></body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", pageTemplate());
}

void handleDado() {
  if (server.hasArg("id") && server.hasArg("rssi")) {
    String id = server.arg("id");
    int rssi = server.arg("rssi").toInt();
    bool found = false;

    for (int i = 0; i < nodeCount; i++) {
      if (nodes[i].id == id) {
        nodes[i].rssi = rssi;
        found = true;
        break;
      }
    }

    if (!found && nodeCount < 3) {
      nodes[nodeCount].id = id;
      nodes[nodeCount].rssi = rssi;
      nodeCount++;
    }

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Faltam argumentos");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado no WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/dado", HTTP_POST, handleDado);
  server.begin();
}

void loop() {
  server.handleClient();
}