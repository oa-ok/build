#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

static FILE *in;

typedef unsigned char byte_t;

static long init_offset = 0x2633;

static void
print_special (unsigned char c)
{
		fprintf( stderr, "<0x%x>", c);
}

static void
dump_string (unsigned char *data, int len)
{
		int i;
		print_special (data[0]);
		for (i = 1; i < len; i++) {
				if (isascii (data[i]))
						fprintf (stderr, "%c", data[i]);
				else switch (data [i]) {
				case 0xc6:
						fprintf (stderr, "<word-break>");
						break;
				default:
						print_special (data[i]);
						break;
				}
		}
		fprintf (stderr, "'\n");
}

static void
dump_hex (unsigned char *data, int len)
{
	while (len > 0) {
		int chunk = len < 16 ? len : 16;
		int i;
		
		for (i = 0; i < chunk; i++)
			fprintf( stderr, "%.2x ", data[i] );
		fprintf( stderr, "| " );
		for (i = 0; i < chunk; i++)
			fprintf( stderr, "%c", data[i] < 127 && data[i] > 30 ? data[i] : '.' );
		fprintf( stderr, "\n" );

		len -= chunk;
		data += 16;
	}
}

typedef enum {
		VALUE_STRING,
		VALUE_BIN
} ValueType;

static int
get_len (byte_t codea, byte_t codeb, ValueType *type)
{
#define MAP(_val,_len) \
		case _val: len = _len; break

		*type = VALUE_BIN;

		int len = -1;
		if (codea == 0x01) {
				switch (codeb) {
				MAP( 0x02, 8 ); /* SSEnd */
				MAP( 0x41, 0 ); MAP( 0x42, 0 );
				MAP( 0x6d, 4 );
				MAP( 0x96, 1 );
				MAP( 0xc0, 0 );
				MAP( 0xf6, 3 );
				default:
						break;
				}
		}
		if (codea == 0x02) {
				switch (codeb) {
				case 0x02:
						len = (byte_t) fgetc(in) + 1;
						*type = VALUE_STRING;
						break;
				case 0x20:
				case 0x26: case 0x27:
				case 0x28: case 0x29: case 0x2a: case 0x2b:
				case 0x2c: case 0x2d: case 0x2e: case 0x2f:
						len = 1;
						break;
				MAP( 0x53, 7 ); /* SSEnd */
				case 0x5c:
				case 0x60:
				case 0x64:
				case 0x6a:
						len = 1;
						break;
				default:
						fprintf( stderr, "Guess (A) length as 1 for 0x%x\n", codeb );
						len = 1;
						break;
				}
		}
		if (codea == 0x08 || codea == 0x09 ||
			codea == 0x0a || codea == 0x0b) {
				switch (codeb) {
				case 0x61:
						len = 0;
						break;
				default:
						break;
				}
		}
		if (len < 0 && codeb == 0x61) {
				fprintf( stderr, "Guess (B) length 0 for 0x%x\n", codea );
				len = 0;
		}
#undef MAP
		return len;
}

int
main (int argc, char **argv)
{
		int die = 0;
		byte_t data[65536] = { 0, };

		in = fopen(argv[1], "r");
		fseek (in, init_offset, SEEK_SET);

		while (!feof(in) && !die) {
				byte_t codea = fgetc (in);
				byte_t codeb = fgetc (in);
				ValueType type;
				int len = get_len (codea, codeb, &type);
				if (len < 0) {
						fprintf (stderr, "Parsing error:\n");
						len = 1024;
						die = 1;
				}
				fread (data, 1, len, in);
				if (type == VALUE_STRING)
				{
						fprintf (stderr, "String: %.2x%.2x: ", codea, codeb);
						dump_string (data, len);
				}
				else
				{
						if (len == 0)
								fprintf (stderr, "Flag: 0x%.2x%.2x\n", codea, codeb);
						else
						{
								fprintf (stderr, "Record: 0x%.2x%.2x (0x%d bytes): ",
										 codea, codeb, len );
								dump_hex (data, len);
						}
				}
		}
		fclose (in);
		return 0;
} 
