//writting
typedef struct BitWriter {
	FILE* file;
	int local_position;
	int global_position;
	char* buf;
} BitWriter;

BitWriter* create_bit_writer(FILE* file);
void write_bit(BitWriter* writer, unsigned long long bit);
void close_bit_writer(BitWriter* writer);

//reading 
extern const unsigned int NOT_EMPTY;
extern const unsigned int EMPTY;
typedef struct BitReader {
	FILE* file;
	int local_position;
	int global_position;
	unsigned char* buf;

	int bits_to_read;
	int bytes_in_buf;
	int max_buf_size;
	
	int state;
} BitReader;

BitReader* create_bit_reader(FILE* file);
int read_bit(BitReader* reader);
