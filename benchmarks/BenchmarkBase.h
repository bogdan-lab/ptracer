#ifndef BENCHMARK_BASE_H
#define BENCHMARK_BASE_H

#include <chrono>
#include <fstream>
#include <string>

class BenchmarkBase {
 private:
  std::string path_ = "./";
  std::string name_ = "LostBenchData";
  std::string git_branch_ = "---";
  std::string git_commit_ = "---";
  std::string compiler_ = "---";
  std::chrono::time_point<std::chrono::system_clock> end_ =
      std::chrono::system_clock::from_time_t(0);
  const std::chrono::time_point<std::chrono::system_clock> start_;

 public:
  BenchmarkBase() = delete;
  BenchmarkBase(std::string g_path_, std::string g_name)
      : path_(std::move(g_path_)),
        name_(std::move(g_name)),
        start_(std::chrono::system_clock::now()) {}

  BenchmarkBase& SetGitBranch(std::string g_git_branch) {
    git_branch_ = std::move(g_git_branch);
    return *this;
  }

  BenchmarkBase& SetGitCommit(std::string g_git_commit) {
    git_commit_ = std::move(g_git_commit);
    return *this;
  }

  BenchmarkBase& SetCompilerInfo(std::string g_info) {
    compiler_ = std::move(g_info);
    return *this;
  }

  const std::string& GetName() const { return name_; }
  const std::string& GetPath() const { return path_; }
  const std::string& GetGitBranch() const { return git_branch_; }
  const std::string& GetGetCommit() const { return git_commit_; }
  const std::string& GetCompilerInfo() const { return compiler_; }

  virtual ~BenchmarkBase() {
    end_ = std::chrono::system_clock::now();
    std::ofstream fout{path_ + name_ + std::string(".txt"), std::ios_base::app};
    if (!fout.tellp()) {
      fout << "#Measure timestamp\tDuration, "
              "s\tGitBranch\tGitCommit\tCompiler\n";
    }
    fout << std::chrono::system_clock::to_time_t(start_) << '\t'
         << std::chrono::duration_cast<std::chrono::seconds>(end_ - start_)
                .count()
         << '\t' << git_branch_ << '\t' << git_commit_ << '\t' << compiler_
         << '\n';
  }

  virtual void CreateUniverse() = 0;
  virtual void MkPicture() = 0;
};

#endif  // BENCHMARK_BASE_H
