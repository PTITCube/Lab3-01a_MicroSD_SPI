/**
 * VÍ DỤ 3-1a: GIAO TIẾP THẺ NHỚ BẰNG SPI THUẦN (SD.h)
 * 
 * Mô tả:
 * Sử dụng thư viện SD gốc (giao tiếp SPI) để ghi log thẻ nhớ.
 * Định nghĩa chuẩn xác chân phần cứng CubeSat: CS=13, SCK=18, MISO=19, MOSI=23.
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN   13
#define SD_SCK_PIN  18
#define SD_MISO_PIN 19
#define SD_MOSI_PIN 23

bool sdReady = false;

// Hàm ghi log thuần bằng thư viện SD.h
void logData(const char* data) {
    if (!sdReady) return;
    File file = SD.open("/log.txt", FILE_APPEND);
    if (!file) {
        Serial.println("[Storage] Failed to open /log.txt for appending");
        return;
    }
    file.println(data);
    file.close();
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n[EXAMPLE] Khởi tạo thẻ nhớ...");
    Serial.println("[Storage] Initializing MicroSD (SPI)...");

    // Khởi tạo SPI bus với chân phần cứng CubeSat
    SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    if (!SD.begin(SD_CS_PIN, SPI, 8000000)) {
        Serial.println("[Storage] MicroSD mount failed");
        sdReady = false;
        return;
    }

    Serial.println("[Storage] MicroSD mount OK");
    sdReady = true;

    // In thông tin thẻ nhớ
    Serial.print("[Storage] Tổng: ");
    Serial.print(SD.totalBytes() / (1024 * 1024));
    Serial.print(" MB, Đã dùng: ");
    Serial.print(SD.usedBytes() / (1024 * 1024));
    Serial.println(" MB");

    // Ghi thử một dòng Boot vào log
    logData("=============================");
    logData("SYSTEM BOOT: CUBESAT V1");
    logData("=============================");
}

void loop() {
    static int counter = 0;

    String data = "Hello world #" + String(counter) + " - Uptime: " + String(millis()) + "ms";
    Serial.println(data);

    logData(data.c_str());

    counter++;
    delay(3000);
}

