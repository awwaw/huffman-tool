//
// Created by aww on 23.10.23.
//
#include "lib/include/huffman.h"

#include <fstream>
#include <iostream>
#include <string>

inline static void print_help() {
  std::cout << "Usage: huffman-tool (-c | -d) "
            << "-i FILE_IN -o FILE_OUT\n"
            << "FLAGS:\n"
            << "\t-h, --help                more information\n"
            << "\t-c, --compress            encode file\n"
            << "\t-d, --decompress          decode file\n"
            << "\t-i, --input FILE_IN       input file\n"
            << "\t-o, --output FILE_OUT     output file\n";
}

int main(int argc, char** argv) {
  if (argc <= 1) {
    std::cerr << "No run arguments provided\n";
    std::cerr << R"(Call "--help" or "-h" for more details.)";
    return 1;
  }

  bool help = false;
  bool compress = false;
  bool decompress = false;
  std::string input_file, output_file;

  for (size_t i = 1; i < argc; i++) {
    std::string argument = argv[i];
    if (argument == "--help" || argument == "-h") {
      help = true;
    } else if (argument == "--compress" || argument == "-c") {
      compress = true;
    } else if (argument == "--decompress" || argument == "-d") {
      decompress = true;
    } else if (argument == "--input") {
      i++;
      if (i >= argc) {
        std::cerr << "No input filename provided\n";
        return 1;
      }
      input_file = argv[i];
    } else if (argument == "--output") {
      i++;
      if (i >= argc) {
        std::cerr << "No output filename provided\n";
        return 1;
      }
      output_file = argv[i];
    } else {
      std::cerr << "Unsupported argument №" << i << "\n";
      return 2;
    }
  }

  if (help) {
    print_help();
    return 0;
  }

  if (compress && decompress) {
    std::cerr << "You must choose to either compress or decompress file, not both at once!\n";
    return 1;
  }

  if (!compress && !decompress) {
    std::cerr << "No run mode was given.\n";
    return 1;
  }

  if (input_file.empty()) {
    std::cerr << "Path to input file was not found!\n";
    return 1;
  }
  if (output_file.empty()) {
    if (compress) {
      output_file = input_file + ".huf";
    } else {
      std::cerr << "Path to output file was not found!n";
    }
  }

  std::ifstream input;
  std::ofstream output;

  try {
    input.open(input_file, std::ios_base::binary);
    if (!input.is_open()) {
      throw std::ios_base::failure("Opening file /" + input_file + "/ failed");
    }
  } catch (const std::exception& e) {
    std::cerr << "Opening file /" + input_file + "/ failed: " << e.what() << '\n';
    return 1;
  }

  try {
    output.open(output_file, std::ios_base::binary | std::ios_base::trunc);
    if (!input.is_open()) {
      throw std::ios_base::failure("Opening file /" + input_file + "/ failed");
    }
  } catch (const std::exception& e) {
    std::cerr << "Opening file /" + input_file + "/ failed: " << e.what() << '\n';
    return 1;
  }

  if (compress) {
    try {
      methods::compress(input, output);
    } catch (const std::exception& e) {
      std::cerr << "Compression failed: " << e.what() << '\n';
      return 1;
    }
  } else {
    try {
      methods::decompress(input, output);
    } catch (const std::exception& e) {
      std::cerr << "Decompression failed: " << e.what() << '\n';
      return 1;
    }
  }
  return 0;
}
