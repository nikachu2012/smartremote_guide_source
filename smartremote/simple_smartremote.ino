#include <WiFi.h>
#include <WebServer.h>
#include <IRremote.hpp>

// Wi-FiのSSIDとパスワードを設定
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";

const int SEND_PIN = 10; // 追加 nnはLEDを接続したピン番号

// Webサーバーのポート番号（80番を使用）
WebServer server(80);

// ルート("/")にアクセスしたときの処理
void handleRoot() {
    // この下の行は解析で出てきたプログラムに変える
    IrSender.sendNEC(0x0000, 0x00, 1);

    server.send(200, "text/plain", "Hello, World!");
}


void setup() {
    Serial.begin(115200);
    IrSender.begin(SEND_PIN); // 追加

    // Wi-Fiに接続
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        // 接続されるまで待つ
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");

    Serial.print("IP address     : ");
    Serial.println(WiFi.localIP());

    Serial.print("Default gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("Subnet mask    : ");
    Serial.println(WiFi.subnetMask());
    
    Serial.print("1st DNS Server : ");
    Serial.println(WiFi.dnsIP(0));     // IPアドレス等の情報を取得


    // /のページが呼ばれたらhandleRootが実行されるように登録
    server.on("/", handleRoot);

    // サーバーを開始
    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    server.handleClient(); // クライアントのリクエストを処理
}