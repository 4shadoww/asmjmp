#include <iostream>
#include <iomanip>

std::string version = "0.1";

std::string usage = " [mode] [arg0] [arg1]\n\ndescription:\n\nThis program has two modes [-a] address and [-o] offset. Input must be hex.\nAddress mode calculates the address of the jmp using jmp address and offset.\nOffset mode calculates offset of jmp instruction's address and address where program jumps.\n\nmodes:\n\t-a\tcalculate address usage: asmjmp -a [address] [offset]\n\t-o\tcalculate offset usage: asmjmp -o [from] [destination]\noptions:\n\t-h, --help \tshow help\n\t-v, --version \tshow version";

void printUsage(std::string name){
  std::cout << "usage: " << name << usage << std::endl;
}

void printVersion(){
  std::cout << "asmjmp version: " << version << std::endl;
}

void calculateAddress(std::string arg1, std::string arg2){
  unsigned short offset;
  int address;
  int destination;
  int offirst;
  bool backward = false;
  std::string hex_value;
  std::stringstream stream;
  // Address to dec
  stream << arg1;
  stream >> std::hex >> address;
  std::stringstream().swap(stream);
  // First hex to dec
  stream << arg2.at(0);
  stream >> std::hex >> offirst;
  std::stringstream().swap(stream);

  // Check is jump negative
  if(offirst > 7) backward = true;

  // Calculate signed offset
  if(backward){
    stream <<  "ff"+arg2;
    stream >> std::hex >> offset;
    std::stringstream().swap(stream);
    destination = address + static_cast<signed short>(offset) + 2;
  // Calculate unsigned offset
  }else{
    stream << arg2;
    stream >> std::hex >> offset;
    std::stringstream().swap(stream);
    destination = address + offset + 2;
  }

  // Dec to hex
  stream << std::hex << destination;
  hex_value = stream.str();
  // Print the address as hex
  std::cout << "0x00" << hex_value << std::endl;
}

int forwardJump(int from, int destination){
  return destination - (from + 2);
}

int backwardJump(int from, int destination){
  return (0 - (from - destination)) - 2;
}

void calculateDistance(std::string arg1, std::string arg2){
  int from, destination;
  short jmp_dis;
  std::string hex_value;
  // Open stringstream
  std::stringstream stream;
  // Convert user input hex string to decimal
  stream << arg1;
  stream >> std::hex >> from;
  std::stringstream().swap(stream);
  stream << arg2;
  stream >> std::hex >> destination;
  std::stringstream().swap(stream);

  // Calculate distance
  if(from < destination){
    jmp_dis = forwardJump(from, destination);
  }else{
    jmp_dis = backwardJump(from, destination);
  }

  // Distance to hex
  stream << std::hex << jmp_dis;
  hex_value = stream.str();

  // Print distance as hex
  std::cout << hex_value << std::endl;
}

int main(int argc, char* argv[]){
  std::string arg0;
  std::string arg1;
  std::string arg2;

  if(argc <= 1){
    printUsage(argv[0]);
    return 1;
  }
  arg0 = argv[1];

  if(arg0 == "--version" || arg0 == "-v"){
    printVersion();
    return 0;
  }

  if(arg0 == "--help" || arg0== "-h"){
    printUsage(argv[0]);
    return 0;
  }

  // Check are arguments valid
  if(argc != 4){
    printUsage(argv[0]);
    return 1;
  }

  arg1 = argv[2];
  arg2 = argv[3];

  if(arg0 == "-a"){
    calculateAddress(arg1, arg2);
  }else if("-o"){
    calculateDistance(arg1, arg2);
  }else{
    printUsage(argv[0]);
    return 1;
  }

  return 0;

}
