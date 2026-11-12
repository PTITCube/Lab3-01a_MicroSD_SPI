/**
 * VÍ DỤ 3-1a: GIAO TIẾP THẺ NHỚ BẰNG SPI THUẦN (SD.h)
 * 
 * Mô tả:
 * Sử dụng thư viện SD gốc (giao tiếp SPI) để ghi log thẻ nhớ.
 * Định nghĩa chuẩn xác chân phần cứng CubeSat: CS=13, SCK=18, MISO=19, MOSI=23.
 * Tích hợp đối tượng cdh (PTIT_CDH) chuẩn hóa phân hệ C&DH.
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <PTITCube.h>

PTIT_CDH cdh;

#define SD_CS_PIN   13
#define SD_SCK_PIN  18
#define SD_MISO_PIN 19
#define SD_MOSI_PIN 23

bool sdReady = false;

void logData(const char* data) {
    if (!sdReady) return;
    File file = SD.open("/log.txt", FILE_APPEND);
    if (!file) {
        cdh.println("[SD] Open /log.txt failed");
        return;
    }
    file.println(data);
    file.close();
}

void setup() {
    cdh.begin(115200);
    while (!cdh) { delay(10); }

    cdh.println("[CDH] MicroSD SPI Init");
    SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    if (!SD.begin(SD_CS_PIN, SPI, 8000000)) {
        cdh.println("[SD] Mount failed");
        sdReady = false;
        return;
    }

    cdh.printf("[SD] OK. Total: %llu MB, Used: %llu MB\n", 
               SD.totalBytes() / (1024 * 1024), 
               SD.usedBytes() / (1024 * 1024));
    sdReady = true;

    logData("BOOT: PTITCUBE C&DH");
}

void loop() {
    static int counter = 0;
    char buf[64];
    snprintf(buf, sizeof(buf), "[DATA] #%d | Uptime: %lums", counter++, millis());
    
    cdh.println(buf);
    logData(buf);

    delay(3000);
}
