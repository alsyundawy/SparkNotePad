class Sparknotepad < Formula
  desc "Lightweight and high-performance native text and source code editor"
  homepage "https://github.com/alsyundawy/SparkNotePad"
  url "https://github.com/alsyundawy/SparkNotePad/archive/refs/tags/v1.3.0.tar.gz"
  sha256 "0000000000000000000000000000000000000000000000000000000000000000"
  license "GPL-3.0-or-later"
  head "https://github.com/alsyundawy/SparkNotePad.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "qt@6"
  depends_on "uchardet"

  def install
    system "cmake", "-S", ".", "-B", "build", *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    assert_match "SparkNotePad", shell_output("#{bin}/sparknotepad --help", 0)
  end
end
