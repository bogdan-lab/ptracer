#ifndef BENCHMARK_BASE_H
#define BENCHMARK_BASE_H

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

class BenchmarkBase {
 private:
  std::string name_ = "UNNAMED_BENCHMARK";
  std::chrono::time_point<std::chrono::system_clock> end_ =
      std::chrono::system_clock::from_time_t(0);
  const std::chrono::time_point<std::chrono::system_clock> start_;

 public:
  BenchmarkBase() = delete;
  BenchmarkBase(std::string g_name)
      : name_(std::move(g_name)), start_(std::chrono::system_clock::now()) {}

  const std::string& GetBenchName() const { return name_; }

  virtual ~BenchmarkBase() {
    end_ = std::chrono::system_clock::now();
    // TODO Path through CMakeSource directory and save benchmark logs there!
    std::string fname = fs::current_path().string() + '/' + name_ + ".txt";
    std::ofstream fout{std::move(fname), std::ios_base::app};
    if (!fout.tellp()) {
      fout << "#Measure timestamp\tDuration, s\n";
    }
    fout << std::chrono::system_clock::to_time_t(start_) << '\t'
         << std::chrono::duration_cast<std::chrono::seconds>(end_ - start_)
                .count()
         << '\n';
  }

  virtual void CreateUniverse() = 0;
  virtual void MkPicture() = 0;
};

#endif  // BENCHMARK_BASE_H
