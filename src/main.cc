#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <cstring>
#include "wagner/common.hh"
#include "wagner/simulation.hh"
#include "wagner/model.hh"

int main(int argc, char *argv[]) {
  size_t nthreads = std::thread::hardware_concurrency(); // number of threads
  wagner::model m = wagner::model::wagner_traits;
  size_t seed = std::random_device{}();
  size_t t_max = (1 << 9);
  size_t communities = 64;
  size_t traits = 10;
  double ext_max = 0.05;
  double mig_max = 0.04;
  double aleph = 10.0;
  double speciation = 0.04;
  double speciation_exp = 1.02;
  double radius = 0.20;
  float white_noise_std = 0.005f;
  bool verbose = false;
  bool shuffle = false;
  bool discard = false;

  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "-model") == 0) {
      const int model_id = atoi(argv[i + 1]);
      switch(model_id) {
        case 0:
          m = wagner::model::wagner_neutral;
          break;
        case 1:
          m = wagner::model::wagner_aleph;
          break;
        case 2:
          m = wagner::model::wagner_logistic;
          break;
        case 3:
          m = wagner::model::wagner_log_aleph;
          break;
        case 4:
        default:
          m = wagner::model::wagner_traits;
      }
    }
    else if (std::strcmp(argv[i], "-threads") == 0)
      nthreads = atoi(argv[i + 1]);
    else if (std::strcmp(argv[i], "-seed") == 0)
      seed = atoi(argv[i + 1]);
    else if (std::strcmp(argv[i], "-n") == 0)
      traits = atoi(argv[i + 1]);
    else if (std::strcmp(argv[i], "-w") == 0)
      white_noise_std = atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-c") == 0)
      communities = atoi(argv[i + 1]);
    else if (std::strcmp(argv[i], "-t") == 0)
      t_max = atoi(argv[i + 1]);
    else if (std::strcmp(argv[i], "-e") == 0)
      ext_max = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-m") == 0)
      mig_max = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-a") == 0)
      aleph = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-s") == 0)
      speciation = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-se") == 0)
      speciation_exp = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-r") == 0)
      radius = std::atof(argv[i + 1]);
    else if (std::strcmp(argv[i], "-verbose") == 0)
      verbose = true;
    else if (std::strcmp(argv[i], "-shuffle") == 0)
      shuffle = true;
    else if (std::strcmp(argv[i], "-discard") == 0)
      discard = true;
  }

  // Force 't_max' to be a power of two:
  if (!power_of_two(t_max)) {
    size_t new_t = 1;
    while (new_t < t_max) {
      new_t <<= 1;
    }
    t_max = (new_t >> 1);
  }

  // Seed the various threads
  std::mt19937_64 rng(seed); // The engine
  std::uniform_int_distribution<size_t> uni;

  std::vector<std::thread> threads;

  // Rather naive but hey, it works fine:
  for (auto i = 0u; i < nthreads; ++i) {
    threads.push_back(
      std::thread(
        wagner::simulation, m, uni(rng), t_max, communities, traits, ext_max,
        mig_max, aleph, speciation, speciation_exp, radius, white_noise_std,
        verbose, shuffle, discard));
  }

  for (auto& thread : threads)
    thread.join();

  return 0;
}
