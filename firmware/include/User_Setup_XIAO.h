#define USER_SETUP_ID 999  // Dowolna liczba — unikalny identyfikator setupu

#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     3
#define TFT_BL     6

#define TFT_SCLK   10
#define TFT_MOSI   8
#define TFT_MISO   -1  // Jeśli nie używasz, ustaw -1

#define ST7735_DRIVER
#define ST7735_GREENTAB160x128  // lub BLACKTAB jeśli taki masz

#define TFT_WIDTH  128
#define TFT_HEIGHT 160

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000

