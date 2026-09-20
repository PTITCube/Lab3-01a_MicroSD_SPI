/**
 * VÍ DỤ 3-1a: GIAO TIẾP THẺ NHỚ BẰNG SPI THUẦN (SD.h)
 */
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN 5

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n[RAW] Khởi tạo thẻ nhớ MicroSD bằng thư viện SD gốc...");
    
    // Khởi tạo SPI cho thẻ nhớ
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("[ERROR] Lỗi! Không tìm thấy thẻ nhớ hoặc chân cắm sai.");
        return;
    }
    
    Serial.println("[RAW] Khởi tạo thành công! Kiểm tra file...");
    
    File file = SD.open("/test.txt", FILE_WRITE);
    if (file) {
        file.println("Hello CubeSat from RAW SPI!");
        file.close();
        Serial.println("[RAW] Đã ghi nội dung thử nghiệm vào /test.txt");
    } else {
        Serial.println("[ERROR] Không thể mở file /test.txt để ghi.");
    }
}

void loop() {
    // Không làm gì
}
