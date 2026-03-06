fname=$1
fout=$2
nasm -f elf64 -g $fname -o EXT.o && \
ld -g -o $fout EXT.o
