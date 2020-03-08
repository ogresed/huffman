#include "headers.h" 

const int BITS_IN_CHAR = 8;
const int local_size = sizeof(char);

// writting
const int wglobal_size = 512;

BitWriter* create_bit_writer(FILE* file) {
	BitWriter* writer = calloc(1, sizeof(BitWriter));
	writer->file = file;
	writer->buf = calloc(wglobal_size, local_size);
	return writer;
}

void write_bit(BitWriter* writer, unsigned long long bit) {
	unsigned char current = writer->buf[writer->global_position];
	unsigned char c_bit = (unsigned char)(bit == 0 ? 0 : 1);
	//changes
	unsigned char bit_in_position = c_bit << BITS_IN_CHAR - writer->local_position - 1;
	unsigned char current_without_bit = current & (~bit_in_position);
	writer->buf[writer->global_position] = current_without_bit | bit_in_position;
	//changes
	writer->local_position++;

	if (writer->local_position == local_size * BITS_IN_CHAR) {
		writer->local_position = 0;
		writer->global_position++;
	}
	// flush if buf is full
	if (writer->global_position == wglobal_size) {
		writer->global_position = 0;
		fwrite(writer->buf, local_size, wglobal_size, writer->file);
		memset(writer->buf, 0, wglobal_size);
	}
}
//close - write endbyte
void close_bit_writer(BitWriter* writer) {
	int size_char = sizeof(char);
	int chars_in_one_typew = local_size / size_char;
	int fulled_chars_in_buf = writer->global_position * chars_in_one_typew +
		(writer->local_position / (BITS_IN_CHAR * size_char)) +
		(writer->local_position % (BITS_IN_CHAR * size_char) == 0 ? 0 : 1);

	fwrite(writer->buf, size_char, fulled_chars_in_buf, writer->file);
	printf("last chars in buffer: %d\n", fulled_chars_in_buf);

	unsigned char writted_bits = writer->local_position;
	// if last byte == 0 then byte before is not exist
	fwrite(&writted_bits, sizeof(char), 1, writer->file);
	printf("bit in last byte: %d\n", writted_bits);
}

//reading
const int rglobal_size = 512;
const unsigned int NOT_EMPTY = 0xffff;
const unsigned int EMPTY = 0x10000;
BitReader* create_bit_reader(FILE* file) {
	BitReader* reader = calloc(1, sizeof(BitReader));
	reader->max_buf_size = rglobal_size;
	reader->file = file;
	reader->buf = calloc(rglobal_size, local_size);
	int readed = fread(reader->buf, local_size, rglobal_size - 1, reader->file);
	reader->bytes_in_buf = readed;

	reader->bits_to_read = BITS_IN_CHAR;
	
	reader->state = NOT_EMPTY;
	
	return reader;
}

int read_bit(BitReader* reader) {
	int masc = 1 << (BITS_IN_CHAR - reader->local_position - 1);
	int bit = (reader->buf[reader->global_position] & masc) == 0 ? 0 : 1;

	reader->local_position++;

	if (reader->local_position == local_size * reader->bits_to_read) {
		reader->local_position = 0;
		reader->global_position++;
	}
	// load new data if buf is fully read
	int number_to_read = reader->bytes_in_buf - 2;
	
	if(number_to_read < reader->global_position) {
		reader->state = EMPTY;
		return bit;
	}
	
	if (number_to_read != 0 && reader->global_position == number_to_read) {
		//printf("number to read  %d\n", number_to_read);
		reader->global_position = 0;
		//printf("___%s\n", reader->buf);
		reader->buf[0] = reader->buf[number_to_read];
		reader->buf[1] = reader->buf[number_to_read + 1];
		
		int readed = fread(reader->buf +  2, local_size, rglobal_size - 3, reader->file);
		
		reader->bytes_in_buf = readed + 2;
		//printf("=== %d \n", reader->bytes_in_buf);
		unsigned int lstate = (unsigned int)reader->buf[1];
		unsigned int last_byte = (unsigned int)reader->buf[0];
		//printf("last byte %u \n",  last_byte);
		//printf("!-state %u \n",  lstate);
		
		if(readed == 0) {
			reader->bits_to_read = lstate == 0 ? 8 : lstate % 8;
			//reader->state = EMPTY;
		}
	}
	return bit;
}
