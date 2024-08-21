#ifndef COLOR_H
#define COLOR_H

typedef struct rgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgbColor;

typedef struct hsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} hsvColor;

rgbColor hsvToRgb(hsvColor hsv);
hsvColor rgbToHsv(rgbColor rgb);

#endif