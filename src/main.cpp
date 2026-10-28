/**
 * VÍ DỤ 3-1a: GIAO TIẾP THẺ NHỚ BẰNG SPI THUẦN (SD.h)
 */
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// Định nghĩa đúng các chân SPI cho module MicroSD trên vệ tinh CubeSat
#define SD_CS_PIN   13
#define SD_SCK_PIN  18
#define SD_MISO_PIN 19
#define SD_MOSI_PIN 23

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n[RAW] Khởi tạo thẻ nhớ MicroSD bằng thư viện SD gốc (SPI)...");
    Serial.printf("[RAW] Cấu hình chân SPI: CS=%d, SCK=%d, MISO=%d, MOSI=%d\n",
                  SD_CS_PIN, SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
    
    // Khởi tạo SPI bus với các chân GPIO thực tế của mạch CubeSat
    SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
    
    // Khởi tạo thẻ nhớ MicroSD qua bus SPI vừa cấu hình
    if (!SD.begin(SD_CS_PIN, SPI, 8000000)) {
        Serial.println("[ERROR] Lỗi! Không tìm thấy thẻ nhớ hoặc chân cắm sai.");
        return;
    }
    
    Serial.println("[RAW] Khởi tạo thẻ nhớ thành công!");

    // Đọc thông tin thẻ nhớ
    uint8_t cardType = SD.cardType();
    Serial.print("[RAW] Loại thẻ: ");
    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SDSC");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("UNKNOWN");

    uint64_t totalMB = SD.totalBytes() / (1024 * 1024);
    uint64_t usedMB = SD.usedBytes() / (1024 * 1024);
    Serial.printf("[RAW] Dung lượng: %llu MB | Đã dùng: %llu MB\n", totalMB, usedMB);
    
    // Thử nghiệm ghi dữ liệu vào file
    File file = SD.open("/test.txt", FILE_WRITE);
    if (file) {
        file.println("Hello CubeSat from RAW SPI (CS=13, SCK=18, MISO=19, MOSI=23)!");
        file.close();
        Serial.println("[RAW] Đã ghi nội dung thử nghiệm vào /test.txt");
    } else {
        Serial.println("[ERROR] Không thể mở file /test.txt để ghi.");
        return;
    }

    // Thử nghiệm đọc lại dữ liệu từ file
    file = SD.open("/test.txt", FILE_READ);
    if (file) {
        Serial.println("[RAW] Đọc nội dung file /test.txt:");
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
        Serial.println();
    } else {
        Serial.println("[ERROR] Không thể mở file /test.txt để đọc.");
    }
}

void loop() {
    delay(1000);
}
