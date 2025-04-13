#include <WiFi.h>
#include <WebServer.h>
#include <IRremote.hpp>
#include 
// Wi-FiのSSIDとパスワードを設定
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";

const int SEND_PIN = 10; // 追加 nnはLEDを接続したピン番号

// xより左はシリアルモニタと同じにする。xは255以下で任意の値 おすすめは200
const IPAddress ip(192, 168, 0, 200); // IPアドレスを設定

const IPAddress gateway(192, 168, 0, 1); // デフォルトゲートウェイ
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク
const IPAddress dns1(192, 168, 0, 1); // DNS

// Webサーバーのポート番号（80番を使用）
WebServer server(80);

// ルート("/")にアクセスしたときの処理
void handleRoot()
{
    // リクエストボディを取得
    String requestBody = server.arg("plain");

    // ArduinoJsonでJSONを解析 512は確保するサイズ
    StaticJsonDocument<512> body;
    auto err = deserializeJson(body, requestBody);
    if (err) // エラーチェック
    {
        server.send(400); // 400 Bad request
        return;
    }
    // int a = body["a"] // { "a": 10 } なら10が取れる

    // ここから返すためのJSONを作成
    JsonDocument data;
    
    // JSONを編集する
    // data["buf"] = 100;

    // JSONを文字列に戻す
    String result;
    serializeJson(data, result);
    
    // HTTPステータスコード200で返す
    server.send(200, "application/json", result);
}

void setup() {
    Serial.begin(115200);
    IrSender.begin(SEND_PIN); // 追加

    // Wi-Fiに接続
    WiFi.begin(ssid, password);

    // この下を追加
    if (!WiFi.config(ip, gateway, subnet, dns1)) // Wi-Fiの設定
    {
        // 設定に失敗した場合
        Serial.println("Failed to configure!");
        for(;;); // 無限ループ
    }

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