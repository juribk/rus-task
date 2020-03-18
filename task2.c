#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


// #include "device.h"
// #include "plib_uart2.h"

#define SIZE_MESSAGE 58
#define SIZE_VALUES SIZE_MESSAGE / 4

// typedef struct {
//     uint16_t temperature; 
//     uint16_t pressure; 
//     uint16_t voltage;
//     uint16_t vibration[10];
// } sensors_t;

double convert_DS18B20 (uint16_t temp) {
    float temp_c = 1;
    if (temp >> 15) {
        temp = (~temp & 0x0FFF) + 1;
        temp_c = -1;
    }
    return (temp_c * (float)temp / 16.0);    
}

uint8_t ChToHex(uint8_t ch) {
	if (ch >= 0x30 && ch <= 0x39)
		return ch - 0x30;
	else
		return ch - 0x37;
}

uint8_t CRC8(const char *input_buff, size_t num_bytes) {
	uint32_t crc = 0x00;

	size_t a, i = 0;
	for (a = 0; a < num_bytes / 2; a++) {
		crc = crc + ((ChToHex(input_buff[i]) << 4) + 
			          ChToHex(input_buff[i + 1]));
		i = i + 2;
	}
	return (uint8_t)(0x100 - (crc & 0x0000ff));
}

int ParceBuf(char* buffer, uint16_t *rez) {
    //_RUS0550279455F000FF01FF02FF03FF04FF05FF06FF07FF08FF09FFF1    
    //_RUS 0550 2794 55F0  00FF 01FF 02FF 03FF 04FF 05FF 06FF 07FF 08FF 09FF F1    
    for(int i = 0; i < SIZE_VALUES; i++) {
        char str[5], *err;
        strncpy(str, buffer, 4);
        str[4] = '\0';
        if (i == 0) {
            if (strcmp(str, "_RUS")) 
                return 0;
        }
        else {
            rez[i] = strtol (str, &err, 16);
            if (strlen(err) > 0) {
                return 0;
            }
        }
        buffer += 4;
    }
    return 1;
}


int main (void) {
    unsigned char read_buff[SIZE_MESSAGE + 1];
    uint16_t rez[SIZE_VALUES];
    // sensors_t sensors;

    // UART2_Initialize();
    // if (UART2_Read(read_buff, SIZE_MESSAGE)) {
        strcpy(read_buff, "_RUS0550279455F000FF01FF02FF03FF04FF05FF06FF07FF08FF09FFf1");
        uint16_t crc = CRC8(read_buff, SIZE_MESSAGE - 2);
        uint16_t buff_crc = strtol (read_buff + SIZE_MESSAGE - 2, NULL, 16);
        if ( buff_crc != crc) {
            printf("Ошибка CRC %x - %x\n", crc, buff_crc);
            return -1;
        }

        printf("Получено сообщение %s\n", read_buff);
        if(ParceBuf(read_buff, rez)) {
            printf("Показание сенсоров...\n");
            printf("Температура: %0.2f °C\n", convert_DS18B20(rez[1]));
            printf("Давление: %0.2f кПа\n", (double)rez[2] / 100.0);
            printf("Напряжение: %0.2f В\n", (double)rez[3] / 100.0);
            printf("Датчик вибрации:\n");
            for(int i = 4; i < SIZE_VALUES - 1; i++) {
                printf("%d: %d\n", i - 4, rez[i]);
            }
        }
        else {
            printf("Ошибка формата данных\n");
            return -1;
        }
    // }
    // else {
    //     printf("Ошибка чтения данных\n");
    //     return -1;
    // }
}