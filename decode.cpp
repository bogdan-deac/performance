#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define OPCODE_MOV 0x88

string reg_table[] = {"al", "ax", "cl", "cx", "dl", "dx", "bl", "bx", "ah", "sp", "ch", "bp", "dh", "si", "bh", "di"};

string decodeOpMov(uint16_t op) {
  uint8_t D, W, MOD, REG, RM;
  D = (op >> 8) & 0x02;
  W = (op >> 8) & 0x01;
  MOD = (op >> 6) & 0x03 ;
  REG = (op >> 3) & 0x07;
  RM = op & 0x07;

  return "mov " + (D == 0 ?
    reg_table[2*RM + W] + " " + reg_table[2*REG +W] :
    reg_table[2*REG +W] + " " + reg_table[2*RM + W]);
}

void decode(vector<char> instructions) {
  uint8_t opcode;
  uint16_t op;

  for(int offset = 0; offset < instructions.size();) {
    op = ((instructions[offset] & 0xff) << 8) | instructions[offset+1] & 0xff;
    opcode = instructions[offset] & 0xfc;
    switch (opcode) {
      case (uint8_t)OPCODE_MOV:
        cout << decodeOpMov(op) << endl;
        offset += 2;
        break;
      default:
        cout << "Operation with opcode " << (uint8_t)opcode << "not supported";
        return;
    }
  }

}


int main(int argc, char **argv){
  if (argc != 2) {
    cout << "Wrong number of arguments, expected 2, got " << argc << endl;
  }

  // open the binary
  ifstream BinaryFile(argv[1], ios::in | ios::binary);
  if (!BinaryFile) {
    cerr << "Failed to open binary file" << endl;
    return -1;
  }

  // get file size
  BinaryFile.seekg(0, ios::end);
  streampos fileSize = BinaryFile.tellg();
  BinaryFile.seekg(0, ios::beg);

  // buffer
  vector<char> buffer(fileSize);

  BinaryFile.read(buffer.data(), fileSize);

  decode(buffer);

  BinaryFile.close();

}
