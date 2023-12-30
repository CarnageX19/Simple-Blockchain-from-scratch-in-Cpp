#include <iostream>
#include <string>
#include <qrencode.h>

using namespace std;
void generateQR(string data)
{   
    QRcode *qrcode;
    
    qrcode = QRcode_encodeString(data.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);

    for (int y = 0; y < qrcode->width; y++) {
        for (int x = 0; x < qrcode->width; x++) {
            if (qrcode->data[y * qrcode->width + x] & 1) {
                std::cout << "##"; // QR code module is black
            } else {
                std::cout << "  "; // QR code module is white
            }
        }
        std::cout << std::endl;
    }

    QRcode_free(qrcode);
}
