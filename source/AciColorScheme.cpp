#include "AciColorScheme.h"

QColor AciColorScheme::getColor(int iColorIndex)
{
	switch(iColorIndex)
	{
		case   0: return qRgb(  0,   0,   0);
		case   1: return qRgb(255,   0,   0);
		case   2: return qRgb(255, 255,   0);
		case   3: return qRgb(  0, 255,   0);
		case   4: return qRgb(  0, 255, 255);
		case   5: return qRgb(  0,   0, 255);
		case   6: return qRgb(255,   0, 255);
		case   7: return qRgb(255, 255, 255);
		case   8: return qRgb(128, 128, 128);
		case   9: return qRgb(192, 192, 192);
		case  10: return qRgb(255,   0,   0);
		case  11: return qRgb(255, 127, 127);
		case  12: return qRgb(204,   0,   0);
		case  13: return qRgb(204, 102, 102);
		case  14: return qRgb(153,   0,   0);
		case  15: return qRgb(153,  76,  76);
		case  16: return qRgb(127,   0,   0);
		case  17: return qRgb(127,  63,  63);
		case  18: return qRgb( 76,   0,   0);
		case  19: return qRgb( 76,  38,  38);
		case  20: return qRgb(255,  63,   0);
		case  21: return qRgb(255, 159, 127);
		case  22: return qRgb(204,  51,   0);
		case  23: return qRgb(204, 127, 102);
		case  24: return qRgb(153,  38,   0);
		case  25: return qRgb(153,  95,  76);
		case  26: return qRgb(127,  31,   0);
		case  27: return qRgb(127,  79,  63);
		case  28: return qRgb( 76,  19,   0);
		case  29: return qRgb( 76,  47,  38);
		case  30: return qRgb(255, 127,   0);
		case  31: return qRgb(255, 191, 127);
		case  32: return qRgb(204, 102,   0);
		case  33: return qRgb(204, 153, 102);
		case  34: return qRgb(153,  76,   0);
		case  35: return qRgb(153, 114,  76);
		case  36: return qRgb(127,  63,   0);
		case  37: return qRgb(127,  95,  63);
		case  38: return qRgb( 76,  38,   0);
		case  39: return qRgb( 76,  57,  38);
		case  40: return qRgb(255, 191,   0);
		case  41: return qRgb(255, 223, 127);
		case  42: return qRgb(204, 153,   0);
		case  43: return qRgb(204, 178, 102);
		case  44: return qRgb(153, 114,   0);
		case  45: return qRgb(153, 133,  76);
		case  46: return qRgb(127,  95,   0);
		case  47: return qRgb(127, 111,  63);
		case  48: return qRgb( 76,  57,   0);
		case  49: return qRgb( 76,  66,  38);
		case  50: return qRgb(255, 255,   0);
		case  51: return qRgb(255, 255, 127);
		case  52: return qRgb(204, 204,   0);
		case  53: return qRgb(204, 204, 102);
		case  54: return qRgb(153, 153,   0);
		case  55: return qRgb(153, 153,  76);
		case  56: return qRgb(127, 127,   0);
		case  57: return qRgb(127, 127,  63);
		case  58: return qRgb( 76,  76,   0);
		case  59: return qRgb( 76,  76,  38);
		case  60: return qRgb(191, 255,   0);
		case  61: return qRgb(223, 255, 127);
		case  62: return qRgb(153, 204,   0);
		case  63: return qRgb(178, 204, 102);
		case  64: return qRgb(114, 153,   0);
		case  65: return qRgb(133, 153,  76);
		case  66: return qRgb( 95, 127,   0);
		case  67: return qRgb(111, 127,  63);
		case  68: return qRgb( 57,  76,   0);
		case  69: return qRgb( 66,  76,  38);
		case  70: return qRgb(127, 255,   0);
		case  71: return qRgb(191, 255, 127);
		case  72: return qRgb(102, 204,   0);
		case  73: return qRgb(153, 204, 102);
		case  74: return qRgb( 76, 153,   0);
		case  75: return qRgb(114, 153,  76);
		case  76: return qRgb( 63, 127,   0);
		case  77: return qRgb( 95, 127,  63);
		case  78: return qRgb( 38,  76,   0);
		case  79: return qRgb( 57,  76,  38);
		case  80: return qRgb( 63, 255,   0);
		case  81: return qRgb(159, 255, 127);
		case  82: return qRgb( 51, 204,   0);
		case  83: return qRgb(127, 204, 102);
		case  84: return qRgb( 38, 153,   0);
		case  85: return qRgb( 95, 153,  76);
		case  86: return qRgb( 31, 127,   0);
		case  87: return qRgb( 79, 127,  63);
		case  88: return qRgb( 19,  76,   0);
		case  89: return qRgb( 47,  76,  38);
		case  90: return qRgb(  0, 255,   0);
		case  91: return qRgb(127, 255, 127);
		case  92: return qRgb(  0, 204,   0);
		case  93: return qRgb(102, 204, 102);
		case  94: return qRgb(  0, 153,   0);
		case  95: return qRgb( 76, 153,  76);
		case  96: return qRgb(  0, 127,   0);
		case  97: return qRgb( 63, 127,  63);
		case  98: return qRgb(  0,  76,   0);
		case  99: return qRgb( 38,  76,  38);
		case 100: return qRgb(  0, 255,  63);
		case 101: return qRgb(127, 255, 159);
		case 102: return qRgb(  0, 204,  51);
		case 103: return qRgb(102, 204, 127);
		case 104: return qRgb(  0, 153,  38);
		case 105: return qRgb( 76, 153,  95);
		case 106: return qRgb(  0, 127,  31);
		case 107: return qRgb( 63, 127,  79);
		case 108: return qRgb(  0,  76,  19);
		case 109: return qRgb( 38,  76,  47);
		case 110: return qRgb(  0, 255, 127);
		case 111: return qRgb(127, 255, 191);
		case 112: return qRgb(  0, 204, 102);
		case 113: return qRgb(102, 204, 153);
		case 114: return qRgb(  0, 153,  76);
		case 115: return qRgb( 76, 153, 114);
		case 116: return qRgb(  0, 127,  63);
		case 117: return qRgb( 63, 127,  95);
		case 118: return qRgb(  0,  76,  38);
		case 119: return qRgb( 38,  76,  57);
		case 120: return qRgb(  0, 255, 191);
		case 121: return qRgb(127, 255, 223);
		case 122: return qRgb(  0, 204, 153);
		case 123: return qRgb(102, 204, 178);
		case 124: return qRgb(  0, 153, 114);
		case 125: return qRgb( 76, 153, 133);
		case 126: return qRgb(  0, 127,  95);
		case 127: return qRgb( 63, 127, 111);
		case 128: return qRgb(  0,  76,  57);
		case 129: return qRgb( 38,  76,  66);
		case 130: return qRgb(  0, 255, 255);
		case 131: return qRgb(127, 255, 255);
		case 132: return qRgb(  0, 204, 204);
		case 133: return qRgb(102, 204, 204);
		case 134: return qRgb(  0, 153, 153);
		case 135: return qRgb( 76, 153, 153);
		case 136: return qRgb(  0, 127, 127);
		case 137: return qRgb( 63, 127, 127);
		case 138: return qRgb(  0,  76,  76);
		case 139: return qRgb( 38,  76,  76);
		case 140: return qRgb(  0, 191, 255);
		case 141: return qRgb(127, 223, 255);
		case 142: return qRgb(  0, 153, 204);
		case 143: return qRgb(102, 178, 204);
		case 144: return qRgb(  0, 114, 153);
		case 145: return qRgb( 76, 133, 153);
		case 146: return qRgb(  0,  95, 127);
		case 147: return qRgb( 63, 111, 127);
		case 148: return qRgb(  0,  57,  76);
		case 149: return qRgb( 38,  66,  76);
		case 150: return qRgb(  0, 127, 255);
		case 151: return qRgb(127, 191, 255);
		case 152: return qRgb(  0, 102, 204);
		case 153: return qRgb(102, 153, 204);
		case 154: return qRgb(  0,  76, 153);
		case 155: return qRgb( 76, 114, 153);
		case 156: return qRgb(  0,  63, 127);
		case 157: return qRgb( 63,  95, 127);
		case 158: return qRgb(  0,  38,  76);
		case 159: return qRgb( 38,  57,  76);
		case 160: return qRgb(  0,  63, 255);
		case 161: return qRgb(127, 159, 255);
		case 162: return qRgb(  0,  51, 204);
		case 163: return qRgb(102, 127, 204);
		case 164: return qRgb(  0,  38, 153);
		case 165: return qRgb( 76,  95, 153);
		case 166: return qRgb(  0,  31, 127);
		case 167: return qRgb( 63,  79, 127);
		case 168: return qRgb(  0,  19,  76);
		case 169: return qRgb( 38,  47,  76);
		case 170: return qRgb(  0,   0, 255);
		case 171: return qRgb(127, 127, 255);
		case 172: return qRgb(  0,   0, 204);
		case 173: return qRgb(102, 102, 204);
		case 174: return qRgb(  0,   0, 153);
		case 175: return qRgb( 76,  76, 153);
		case 176: return qRgb(  0,   0, 127);
		case 177: return qRgb( 63,  63, 127);
		case 178: return qRgb(  0,   0,  76);
		case 179: return qRgb( 38,  38,  76);
		case 180: return qRgb( 63,   0, 255);
		case 181: return qRgb(159, 127, 255);
		case 182: return qRgb( 51,   0, 204);
		case 183: return qRgb(127, 102, 204);
		case 184: return qRgb( 38,   0, 153);
		case 185: return qRgb( 95,  76, 153);
		case 186: return qRgb( 31,   0, 127);
		case 187: return qRgb( 79,  63, 127);
		case 188: return qRgb( 19,   0,  76);
		case 189: return qRgb( 47,  38,  76);
		case 190: return qRgb(127,   0, 255);
		case 191: return qRgb(191, 127, 255);
		case 192: return qRgb(102,   0, 204);
		case 193: return qRgb(153, 102, 204);
		case 194: return qRgb( 76,   0, 153);
		case 195: return qRgb(114,  76, 153);
		case 196: return qRgb( 63,   0, 127);
		case 197: return qRgb( 95,  63, 127);
		case 198: return qRgb( 38,   0,  76);
		case 199: return qRgb( 57,  38,  76);
		case 200: return qRgb(191,   0, 255);
		case 201: return qRgb(223, 127, 255);
		case 202: return qRgb(153,   0, 204);
		case 203: return qRgb(178, 102, 204);
		case 204: return qRgb(114,   0, 153);
		case 205: return qRgb(133,  76, 153);
		case 206: return qRgb( 95,   0, 127);
		case 207: return qRgb(111,  63, 127);
		case 208: return qRgb( 57,   0,  76);
		case 209: return qRgb( 66,  38,  76);
		case 210: return qRgb(255,   0, 255);
		case 211: return qRgb(255, 127, 255);
		case 212: return qRgb(204,   0, 204);
		case 213: return qRgb(204, 102, 204);
		case 214: return qRgb(153,   0, 153);
		case 215: return qRgb(153,  76, 153);
		case 216: return qRgb(127,   0, 127);
		case 217: return qRgb(127,  63, 127);
		case 218: return qRgb( 76,   0,  76);
		case 219: return qRgb( 76,  38,  76);
		case 220: return qRgb(255,   0, 191);
		case 221: return qRgb(255, 127, 223);
		case 222: return qRgb(204,   0, 153);
		case 223: return qRgb(204, 102, 178);
		case 224: return qRgb(153,   0, 114);
		case 225: return qRgb(153,  76, 133);
		case 226: return qRgb(127,   0,  95);
		case 227: return qRgb(127,  63, 111);
		case 228: return qRgb( 76,   0,  57);
		case 229: return qRgb( 76,  38,  66);
		case 230: return qRgb(255,   0, 127);
		case 231: return qRgb(255, 127, 191);
		case 232: return qRgb(204,   0, 102);
		case 233: return qRgb(204, 102, 153);
		case 234: return qRgb(153,   0,  76);
		case 235: return qRgb(153,  76, 114);
		case 236: return qRgb(127,   0,  63);
		case 237: return qRgb(127,  63,  95);
		case 238: return qRgb( 76,   0,  38);
		case 239: return qRgb( 76,  38,  57);
		case 240: return qRgb(255,   0,  63);
		case 241: return qRgb(255, 127, 159);
		case 242: return qRgb(204,   0,  51);
		case 243: return qRgb(204, 102, 127);
		case 244: return qRgb(153,   0,  38);
		case 245: return qRgb(153,  76,  95);
		case 246: return qRgb(127,   0,  31);
		case 247: return qRgb(127,  63,  79);
		case 248: return qRgb( 76,   0,  19);
		case 249: return qRgb( 76,  38,  47);
		case 250: return qRgb( 51,  51,  51);
		case 251: return qRgb( 91,  91,  91);
		case 252: return qRgb(132, 132, 132);
		case 253: return qRgb(173, 173, 173);
		case 254: return qRgb(214, 214, 214);
		case 255: return qRgb(255, 255, 255);
		default: return Qt::white;
	}
}

int AciColorScheme::getColorIndex(const QColor &color)
{
	if(QColor(qRgb(  0,   0,   0)) == color) return   0;
	if(QColor(qRgb(255,   0,   0)) == color) return   1;
	if(QColor(qRgb(255, 255,   0)) == color) return   2;
	if(QColor(qRgb(  0, 255,   0)) == color) return   3;
	if(QColor(qRgb(  0, 255, 255)) == color) return   4;
	if(QColor(qRgb(  0,   0, 255)) == color) return   5;
	if(QColor(qRgb(255,   0, 255)) == color) return   6;
	if(QColor(qRgb(255, 255, 255)) == color) return   7;
	if(QColor(qRgb(128, 128, 128)) == color) return   8;
	if(QColor(qRgb(192, 192, 192)) == color) return   9;
	if(QColor(qRgb(255,   0,   0)) == color) return  10;
	if(QColor(qRgb(255, 127, 127)) == color) return  11;
	if(QColor(qRgb(204,   0,   0)) == color) return  12;
	if(QColor(qRgb(204, 102, 102)) == color) return  13;
	if(QColor(qRgb(153,   0,   0)) == color) return  14;
	if(QColor(qRgb(153,  76,  76)) == color) return  15;
	if(QColor(qRgb(127,   0,   0)) == color) return  16;
	if(QColor(qRgb(127,  63,  63)) == color) return  17;
	if(QColor(qRgb( 76,   0,   0)) == color) return  18;
	if(QColor(qRgb( 76,  38,  38)) == color) return  19;
	if(QColor(qRgb(255,  63,   0)) == color) return  20;
	if(QColor(qRgb(255, 159, 127)) == color) return  21;
	if(QColor(qRgb(204,  51,   0)) == color) return  22;
	if(QColor(qRgb(204, 127, 102)) == color) return  23;
	if(QColor(qRgb(153,  38,   0)) == color) return  24;
	if(QColor(qRgb(153,  95,  76)) == color) return  25;
	if(QColor(qRgb(127,  31,   0)) == color) return  26;
	if(QColor(qRgb(127,  79,  63)) == color) return  27;
	if(QColor(qRgb( 76,  19,   0)) == color) return  28;
	if(QColor(qRgb( 76,  47,  38)) == color) return  29;
	if(QColor(qRgb(255, 127,   0)) == color) return  30;
	if(QColor(qRgb(255, 191, 127)) == color) return  31;
	if(QColor(qRgb(204, 102,   0)) == color) return  32;
	if(QColor(qRgb(204, 153, 102)) == color) return  33;
	if(QColor(qRgb(153,  76,   0)) == color) return  34;
	if(QColor(qRgb(153, 114,  76)) == color) return  35;
	if(QColor(qRgb(127,  63,   0)) == color) return  36;
	if(QColor(qRgb(127,  95,  63)) == color) return  37;
	if(QColor(qRgb( 76,  38,   0)) == color) return  38;
	if(QColor(qRgb( 76,  57,  38)) == color) return  39;
	if(QColor(qRgb(255, 191,   0)) == color) return  40;
	if(QColor(qRgb(255, 223, 127)) == color) return  41;
	if(QColor(qRgb(204, 153,   0)) == color) return  42;
	if(QColor(qRgb(204, 178, 102)) == color) return  43;
	if(QColor(qRgb(153, 114,   0)) == color) return  44;
	if(QColor(qRgb(153, 133,  76)) == color) return  45;
	if(QColor(qRgb(127,  95,   0)) == color) return  46;
	if(QColor(qRgb(127, 111,  63)) == color) return  47;
	if(QColor(qRgb( 76,  57,   0)) == color) return  48;
	if(QColor(qRgb( 76,  66,  38)) == color) return  49;
	if(QColor(qRgb(255, 255,   0)) == color) return  50;
	if(QColor(qRgb(255, 255, 127)) == color) return  51;
	if(QColor(qRgb(204, 204,   0)) == color) return  52;
	if(QColor(qRgb(204, 204, 102)) == color) return  53;
	if(QColor(qRgb(153, 153,   0)) == color) return  54;
	if(QColor(qRgb(153, 153,  76)) == color) return  55;
	if(QColor(qRgb(127, 127,   0)) == color) return  56;
	if(QColor(qRgb(127, 127,  63)) == color) return  57;
	if(QColor(qRgb( 76,  76,   0)) == color) return  58;
	if(QColor(qRgb( 76,  76,  38)) == color) return  59;
	if(QColor(qRgb(191, 255,   0)) == color) return  60;
	if(QColor(qRgb(223, 255, 127)) == color) return  61;
	if(QColor(qRgb(153, 204,   0)) == color) return  62;
	if(QColor(qRgb(178, 204, 102)) == color) return  63;
	if(QColor(qRgb(114, 153,   0)) == color) return  64;
	if(QColor(qRgb(133, 153,  76)) == color) return  65;
	if(QColor(qRgb( 95, 127,   0)) == color) return  66;
	if(QColor(qRgb(111, 127,  63)) == color) return  67;
	if(QColor(qRgb( 57,  76,   0)) == color) return  68;
	if(QColor(qRgb( 66,  76,  38)) == color) return  69;
	if(QColor(qRgb(127, 255,   0)) == color) return  70;
	if(QColor(qRgb(191, 255, 127)) == color) return  71;
	if(QColor(qRgb(102, 204,   0)) == color) return  72;
	if(QColor(qRgb(153, 204, 102)) == color) return  73;
	if(QColor(qRgb( 76, 153,   0)) == color) return  74;
	if(QColor(qRgb(114, 153,  76)) == color) return  75;
	if(QColor(qRgb( 63, 127,   0)) == color) return  76;
	if(QColor(qRgb( 95, 127,  63)) == color) return  77;
	if(QColor(qRgb( 38,  76,   0)) == color) return  78;
	if(QColor(qRgb( 57,  76,  38)) == color) return  79;
	if(QColor(qRgb( 63, 255,   0)) == color) return  80;
	if(QColor(qRgb(159, 255, 127)) == color) return  81;
	if(QColor(qRgb( 51, 204,   0)) == color) return  82;
	if(QColor(qRgb(127, 204, 102)) == color) return  83;
	if(QColor(qRgb( 38, 153,   0)) == color) return  84;
	if(QColor(qRgb( 95, 153,  76)) == color) return  85;
	if(QColor(qRgb( 31, 127,   0)) == color) return  86;
	if(QColor(qRgb( 79, 127,  63)) == color) return  87;
	if(QColor(qRgb( 19,  76,   0)) == color) return  88;
	if(QColor(qRgb( 47,  76,  38)) == color) return  89;
	if(QColor(qRgb(  0, 255,   0)) == color) return  90;
	if(QColor(qRgb(127, 255, 127)) == color) return  91;
	if(QColor(qRgb(  0, 204,   0)) == color) return  92;
	if(QColor(qRgb(102, 204, 102)) == color) return  93;
	if(QColor(qRgb(  0, 153,   0)) == color) return  94;
	if(QColor(qRgb( 76, 153,  76)) == color) return  95;
	if(QColor(qRgb(  0, 127,   0)) == color) return  96;
	if(QColor(qRgb( 63, 127,  63)) == color) return  97;
	if(QColor(qRgb(  0,  76,   0)) == color) return  98;
	if(QColor(qRgb( 38,  76,  38)) == color) return  99;
	if(QColor(qRgb(  0, 255,  63)) == color) return 100;
	if(QColor(qRgb(127, 255, 159)) == color) return 101;
	if(QColor(qRgb(  0, 204,  51)) == color) return 102;
	if(QColor(qRgb(102, 204, 127)) == color) return 103;
	if(QColor(qRgb(  0, 153,  38)) == color) return 104;
	if(QColor(qRgb( 76, 153,  95)) == color) return 105;
	if(QColor(qRgb(  0, 127,  31)) == color) return 106;
	if(QColor(qRgb( 63, 127,  79)) == color) return 107;
	if(QColor(qRgb(  0,  76,  19)) == color) return 108;
	if(QColor(qRgb( 38,  76,  47)) == color) return 109;
	if(QColor(qRgb(  0, 255, 127)) == color) return 110;
	if(QColor(qRgb(127, 255, 191)) == color) return 111;
	if(QColor(qRgb(  0, 204, 102)) == color) return 112;
	if(QColor(qRgb(102, 204, 153)) == color) return 113;
	if(QColor(qRgb(  0, 153,  76)) == color) return 114;
	if(QColor(qRgb( 76, 153, 114)) == color) return 115;
	if(QColor(qRgb(  0, 127,  63)) == color) return 116;
	if(QColor(qRgb( 63, 127,  95)) == color) return 117;
	if(QColor(qRgb(  0,  76,  38)) == color) return 118;
	if(QColor(qRgb( 38,  76,  57)) == color) return 119;
	if(QColor(qRgb(  0, 255, 191)) == color) return 120;
	if(QColor(qRgb(127, 255, 223)) == color) return 121;
	if(QColor(qRgb(  0, 204, 153)) == color) return 122;
	if(QColor(qRgb(102, 204, 178)) == color) return 123;
	if(QColor(qRgb(  0, 153, 114)) == color) return 124;
	if(QColor(qRgb( 76, 153, 133)) == color) return 125;
	if(QColor(qRgb(  0, 127,  95)) == color) return 126;
	if(QColor(qRgb( 63, 127, 111)) == color) return 127;
	if(QColor(qRgb(  0,  76,  57)) == color) return 128;
	if(QColor(qRgb( 38,  76,  66)) == color) return 129;
	if(QColor(qRgb(  0, 255, 255)) == color) return 130;
	if(QColor(qRgb(127, 255, 255)) == color) return 131;
	if(QColor(qRgb(  0, 204, 204)) == color) return 132;
	if(QColor(qRgb(102, 204, 204)) == color) return 133;
	if(QColor(qRgb(  0, 153, 153)) == color) return 134;
	if(QColor(qRgb( 76, 153, 153)) == color) return 135;
	if(QColor(qRgb(  0, 127, 127)) == color) return 136;
	if(QColor(qRgb( 63, 127, 127)) == color) return 137;
	if(QColor(qRgb(  0,  76,  76)) == color) return 138;
	if(QColor(qRgb( 38,  76,  76)) == color) return 139;
	if(QColor(qRgb(  0, 191, 255)) == color) return 140;
	if(QColor(qRgb(127, 223, 255)) == color) return 141;
	if(QColor(qRgb(  0, 153, 204)) == color) return 142;
	if(QColor(qRgb(102, 178, 204)) == color) return 143;
	if(QColor(qRgb(  0, 114, 153)) == color) return 144;
	if(QColor(qRgb( 76, 133, 153)) == color) return 145;
	if(QColor(qRgb(  0,  95, 127)) == color) return 146;
	if(QColor(qRgb( 63, 111, 127)) == color) return 147;
	if(QColor(qRgb(  0,  57,  76)) == color) return 148;
	if(QColor(qRgb( 38,  66,  76)) == color) return 149;
	if(QColor(qRgb(  0, 127, 255)) == color) return 150;
	if(QColor(qRgb(127, 191, 255)) == color) return 151;
	if(QColor(qRgb(  0, 102, 204)) == color) return 152;
	if(QColor(qRgb(102, 153, 204)) == color) return 153;
	if(QColor(qRgb(  0,  76, 153)) == color) return 154;
	if(QColor(qRgb( 76, 114, 153)) == color) return 155;
	if(QColor(qRgb(  0,  63, 127)) == color) return 156;
	if(QColor(qRgb( 63,  95, 127)) == color) return 157;
	if(QColor(qRgb(  0,  38,  76)) == color) return 158;
	if(QColor(qRgb( 38,  57,  76)) == color) return 159;
	if(QColor(qRgb(  0,  63, 255)) == color) return 160;
	if(QColor(qRgb(127, 159, 255)) == color) return 161;
	if(QColor(qRgb(  0,  51, 204)) == color) return 162;
	if(QColor(qRgb(102, 127, 204)) == color) return 163;
	if(QColor(qRgb(  0,  38, 153)) == color) return 164;
	if(QColor(qRgb( 76,  95, 153)) == color) return 165;
	if(QColor(qRgb(  0,  31, 127)) == color) return 166;
	if(QColor(qRgb( 63,  79, 127)) == color) return 167;
	if(QColor(qRgb(  0,  19,  76)) == color) return 168;
	if(QColor(qRgb( 38,  47,  76)) == color) return 169;
	if(QColor(qRgb(  0,   0, 255)) == color) return 170;
	if(QColor(qRgb(127, 127, 255)) == color) return 171;
	if(QColor(qRgb(  0,   0, 204)) == color) return 172;
	if(QColor(qRgb(102, 102, 204)) == color) return 173;
	if(QColor(qRgb(  0,   0, 153)) == color) return 174;
	if(QColor(qRgb( 76,  76, 153)) == color) return 175;
	if(QColor(qRgb(  0,   0, 127)) == color) return 176;
	if(QColor(qRgb( 63,  63, 127)) == color) return 177;
	if(QColor(qRgb(  0,   0,  76)) == color) return 178;
	if(QColor(qRgb( 38,  38,  76)) == color) return 179;
	if(QColor(qRgb( 63,   0, 255)) == color) return 180;
	if(QColor(qRgb(159, 127, 255)) == color) return 181;
	if(QColor(qRgb( 51,   0, 204)) == color) return 182;
	if(QColor(qRgb(127, 102, 204)) == color) return 183;
	if(QColor(qRgb( 38,   0, 153)) == color) return 184;
	if(QColor(qRgb( 95,  76, 153)) == color) return 185;
	if(QColor(qRgb( 31,   0, 127)) == color) return 186;
	if(QColor(qRgb( 79,  63, 127)) == color) return 187;
	if(QColor(qRgb( 19,   0,  76)) == color) return 188;
	if(QColor(qRgb( 47,  38,  76)) == color) return 189;
	if(QColor(qRgb(127,   0, 255)) == color) return 190;
	if(QColor(qRgb(191, 127, 255)) == color) return 191;
	if(QColor(qRgb(102,   0, 204)) == color) return 192;
	if(QColor(qRgb(153, 102, 204)) == color) return 193;
	if(QColor(qRgb( 76,   0, 153)) == color) return 194;
	if(QColor(qRgb(114,  76, 153)) == color) return 195;
	if(QColor(qRgb( 63,   0, 127)) == color) return 196;
	if(QColor(qRgb( 95,  63, 127)) == color) return 197;
	if(QColor(qRgb( 38,   0,  76)) == color) return 198;
	if(QColor(qRgb( 57,  38,  76)) == color) return 199;
	if(QColor(qRgb(191,   0, 255)) == color) return 200;
	if(QColor(qRgb(223, 127, 255)) == color) return 201;
	if(QColor(qRgb(153,   0, 204)) == color) return 202;
	if(QColor(qRgb(178, 102, 204)) == color) return 203;
	if(QColor(qRgb(114,   0, 153)) == color) return 204;
	if(QColor(qRgb(133,  76, 153)) == color) return 205;
	if(QColor(qRgb( 95,   0, 127)) == color) return 206;
	if(QColor(qRgb(111,  63, 127)) == color) return 207;
	if(QColor(qRgb( 57,   0,  76)) == color) return 208;
	if(QColor(qRgb( 66,  38,  76)) == color) return 209;
	if(QColor(qRgb(255,   0, 255)) == color) return 210;
	if(QColor(qRgb(255, 127, 255)) == color) return 211;
	if(QColor(qRgb(204,   0, 204)) == color) return 212;
	if(QColor(qRgb(204, 102, 204)) == color) return 213;
	if(QColor(qRgb(153,   0, 153)) == color) return 214;
	if(QColor(qRgb(153,  76, 153)) == color) return 215;
	if(QColor(qRgb(127,   0, 127)) == color) return 216;
	if(QColor(qRgb(127,  63, 127)) == color) return 217;
	if(QColor(qRgb( 76,   0,  76)) == color) return 218;
	if(QColor(qRgb( 76,  38,  76)) == color) return 219;
	if(QColor(qRgb(255,   0, 191)) == color) return 220;
	if(QColor(qRgb(255, 127, 223)) == color) return 221;
	if(QColor(qRgb(204,   0, 153)) == color) return 222;
	if(QColor(qRgb(204, 102, 178)) == color) return 223;
	if(QColor(qRgb(153,   0, 114)) == color) return 224;
	if(QColor(qRgb(153,  76, 133)) == color) return 225;
	if(QColor(qRgb(127,   0,  95)) == color) return 226;
	if(QColor(qRgb(127,  63, 111)) == color) return 227;
	if(QColor(qRgb( 76,   0,  57)) == color) return 228;
	if(QColor(qRgb( 76,  38,  66)) == color) return 229;
	if(QColor(qRgb(255,   0, 127)) == color) return 230;
	if(QColor(qRgb(255, 127, 191)) == color) return 231;
	if(QColor(qRgb(204,   0, 102)) == color) return 232;
	if(QColor(qRgb(204, 102, 153)) == color) return 233;
	if(QColor(qRgb(153,   0,  76)) == color) return 234;
	if(QColor(qRgb(153,  76, 114)) == color) return 235;
	if(QColor(qRgb(127,   0,  63)) == color) return 236;
	if(QColor(qRgb(127,  63,  95)) == color) return 237;
	if(QColor(qRgb( 76,   0,  38)) == color) return 238;
	if(QColor(qRgb( 76,  38,  57)) == color) return 239;
	if(QColor(qRgb(255,   0,  63)) == color) return 240;
	if(QColor(qRgb(255, 127, 159)) == color) return 241;
	if(QColor(qRgb(204,   0,  51)) == color) return 242;
	if(QColor(qRgb(204, 102, 127)) == color) return 243;
	if(QColor(qRgb(153,   0,  38)) == color) return 244;
	if(QColor(qRgb(153,  76,  95)) == color) return 245;
	if(QColor(qRgb(127,   0,  31)) == color) return 246;
	if(QColor(qRgb(127,  63,  79)) == color) return 247;
	if(QColor(qRgb( 76,   0,  19)) == color) return 248;
	if(QColor(qRgb( 76,  38,  47)) == color) return 249;
	if(QColor(qRgb( 51,  51,  51)) == color) return 250;
	if(QColor(qRgb( 91,  91,  91)) == color) return 251;
	if(QColor(qRgb(132, 132, 132)) == color) return 252;
	if(QColor(qRgb(173, 173, 173)) == color) return 253;
	if(QColor(qRgb(214, 214, 214)) == color) return 254;
	if(QColor(qRgb(255, 255, 255)) == color) return 255;
	return 0;
}
