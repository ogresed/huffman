# huffman

This project is a implementation of Huffman's algorithm for codding and decodding files.

## Installation

First install GNU, then use simple commang for building:

```bash
gcc *.c
```

## Usage

For example:

```bash
cat > test
Mama myla Milu mylom mama mylo uronila

./a.txt -c test codded_test
./a.txt -d codded_test decodded
cat decodded
Mama myla Milu mylom mama mylo uronila
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
