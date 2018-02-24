#include <iostream>
#include <iomanip>

std::string version = "0.2";

std::string usage = " [mode] [arg0] [arg1]\n\ndescription:\n\nThis program has two modes [-a] address and [-o] offset. Input must be hex.\nAddress mode calculates the address of the jmp using jmp address and offset.\nOffset mode calculates offset of jmp instruction's address and address where program jumps.\n\nmodes:\n\t-a\tcalculate address usage: asmjmp -a [address] [offset]\n\t-o\tcalculate offset usage: asmjmp -o [from] [destination]\noptions:\n\t-h, --help \tshow help\n\t-v, --version \tshow version";

void printUsage(std::string name){
  std::cout << "usage: " << name << usage << std::endl;
}

void printVersion(){
  std::cout << "asmjmp version: " << version << std::endl;
}

std::string findEnd(std::string value){
  std::string final_value = "";
  int delta;
  for(int i = 0; i < value.size(); i += 2){
    if(value[i] == '0' && value[i+1] == '0' || std::tolower(value[i]) == 'f' && std::tolower(value[i+1]) == 'f'){
      break;
    }
    final_value += value[i];
    final_value += value[i+1];
  }
  delta = value.size() - final_value.size();
  for(int i = 0; i < delta; i++){
    final_value = "f"+final_value;
  }
  return final_value;
}

void calculateAddress(std::string arg1, std::string arg2, int p){
  unsigned short offset0;
  uint32_t offset1;
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
  if(p == 2){
    stream << arg2.at(0);
    stream >> std::hex >> offirst;
    std::stringstream().swap(stream);
    // Check is jump negative
    if(offirst > 7) backward = true;
  }else{
    if(std::tolower(arg2.at(arg2.size()-1)) == 'f' && std::tolower(arg2.at(arg2.size()-2)) == 'f') backward = true;
  }
  // Calculate with signed offset
  if(backward){
    if(p == 2){
      stream << "ff"+arg2;
      stream >> std::hex >> offset0;
    }else{
      arg2 = findEnd(arg2);
      stream << arg2;
      stream >> std::hex >> offset1;
    }
    std::stringstream().swap(stream);
    if(p == 2){
      destination = address + static_cast<signed short>(offset0) + p;
    }else{
      destination = address + static_cast<int32_t>(offset1) + p;
    }
  // Calculate with unsigned offset
  }else{
    stream << arg2;
    stream >> std::hex >> offset0;
    std::stringstream().swap(stream);
    destination = address + offset0 + p;
  }
  // Dec to hex
  stream << std::hex << destination;
  hex_value = stream.str();
  // Print the address as hex
  std::cout << hex_value << std::endl;
}

int forwardJump(int from, int destination, int p){
  return destination - (from + p);
}

int backwardJump(int from, int destination, int p){
  return (0 - (from - destination)) - p;
}

void calculateDistance(std::string arg1, std::string arg2, int p){
  int from, destination;
  int jmp_dis;
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
    jmp_dis = forwardJump(from, destination, p);
  }else{
    jmp_dis = backwardJump(from, destination, p);
  }

  // Distance to hex
  stream << std::hex << jmp_dis;
  hex_value = stream.str();

  // Print distance as hex
  std::cout << hex_value << std::endl;
}

int main(int argc, char* argv[]){
  std::stringstream stream;
  std::string arg0;
  std::string arg1;
  std::string arg2;
  int p = 2;

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
  if(argc < 4){
    printUsage(argv[0]);
    return 1;
  }

  arg1 = argv[2];
  arg2 = argv[3];

  if(argc > 4){
    p = argv[4][0] - '0';
  }

  if(arg0 == "-a"){
    calculateAddress(arg1, arg2, p);
  }else if(arg0 == "-o"){
    calculateDistance(arg1, arg2, p);
  }else{
    printUsage(argv[0]);
    return 1;
  }

  return 0;

}
