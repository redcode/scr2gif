/*-----------------------------------------------------------------------------.
|                                                                              |
|  scr2gif                                                                     |
|  Copyright (C) 2022 Manuel Sainz de Baranda y Goñi.                          |
|                                                                              |
|  This program is free software: you can redistribute it and/or modify it     |
|  under the terms of the GNU General Public License as published by the Free  |
|  Software Foundation, either version 3 of the License, or (at your option)   |
|  any later version.                                                          |
|                                                                              |
|  This program is distributed in the hope that it will be useful, but         |
|  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
|  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License     |
|  for more details.                                                           |
|                                                                              |
|  You should have received a copy of the GNU General Public License along     |
|  with this program. If not, see <http://www.gnu.org/licenses/>.              |
|                                                                              |
'=============================================================================*/

#include <gif_lib.h>
#include <stdio.h>
#include <stddef.h>

static unsigned char scr[6912];

static GifColorType const palette[16] = {
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0xD7},
	{0xD7, 0x00, 0x00}, {0xD7, 0x00, 0xD7},
	{0x00, 0xD7, 0x00}, {0x00, 0xD7, 0xD7},
	{0xD7, 0xD7, 0x00}, {0xD7, 0xD7, 0xD7},
	{0x00, 0x00, 0x00}, {0x00, 0x00, 0xFF},
	{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0xFF},
	{0x00, 0xFF, 0x00}, {0x00, 0xFF, 0xFF},
	{0xFF, 0xFF, 0x00}, {0xFF, 0xFF, 0xFF}
};

#define VRAM_CHARACTER_INDEX(x, y) \
	(((y & 0xC0) << 5) | ((y & 7) << 8) | ((y & 0x38) << 2) | x)

#define VRAM_ATTRIBUTE_INDEX(x, y) \
	(((y & 0xF8) << 2) + x)


int main(int argc, char **argv)
	{
	FILE *file;
	GifFileType *gif;
	ColorMapObject *color_map;
	int error;
	unsigned int x, y;

	if (argc != 3)
		{
		fputs("Usage: scr2gif <input file> <output file>\n", stderr);
		return -1;
		}

	if ((file = fopen(argv[1], "rb")) == NULL)
		{
		fputs("Error: Can't open input file.\n", stderr);
		return -1;
		}

	fseek(file, 0, SEEK_END);

	if (ftell(file) != 6912)
		{
		fputs("Error: The input file is not a ZX Spectrum screen.\n", stderr);
		return -1;
		}

	fseek(file, 0, SEEK_SET);

	if (fread(scr, 6912, 1, file) != 1)
		{
		fputs("Error: Unespected error while reading input file.\n", stderr);
		return -1;
		}

	fclose(file);

	if ((gif = EGifOpenFileName(argv[2], 0, &error)) == NULL)
		{
		fputs("Error: Can't open output file.\n", stderr);
		return -1;
		}

	color_map = GifMakeMapObject(16, palette);
	EGifPutScreenDesc(gif, 256, 192, 8, 0, color_map);
	EGifPutImageDesc(gif, 0, 0, 256, 192, 0, NULL);

	for (y = 0; y < 192; y++) for (x = 0; x < 32; x++)
		{
		unsigned char
		pixel_mask = 128,
		cell       = scr[VRAM_CHARACTER_INDEX(x, y)],
		attribute  = scr[VRAM_ATTRIBUTE_INDEX(x, y) + 0x1800],
		bright     = (attribute >> 3) & 8,
		colors[2];

		colors[0] = bright | attribute        & 7;
		colors[1] = bright | (attribute >> 3) & 7;

		for (;pixel_mask; pixel_mask >>= 1)
			EGifPutPixel(gif, colors[!(cell & pixel_mask)]);
		}

	GifFreeMapObject(color_map);
	EGifCloseFile(gif, &error);

	return 0;
	}


/* scr2gif.c EOF */
