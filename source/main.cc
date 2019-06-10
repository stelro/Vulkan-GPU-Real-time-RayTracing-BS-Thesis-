#include "RayEngine.hh"
#include <iostream>

using ost::RayEngine;

int main(int argc, char *argv[]) {
//
//  if ( argc < 2 ) {
//    std::cerr << "Usage: " << argv[0] << " NAME" << '\n';
//    return 1;
//  }

  auto *rayengine = new RayEngine("./../scenes/cornell_box_white.json");
 // auto *rayengine = new RayEngine(argv[1]);
  rayengine->prepare();
  rayengine->engineLoop();
  delete (rayengine);
  return EXIT_SUCCESS;
}
