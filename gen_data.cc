//
// Created by liuzihe on 2020/10/14.
//

#include <iostream>

#include "util/file.h"

int main() {
  std::cout << "Hello, World! gen_data" << std::endl;


  tinykv::WritableFile* wf;
  tinykv::NewWritableFile("test.bin", &wf);
  wf->Append("test abc def 123");
  wf->Close();

  tinykv::RandomAccessFile* acf;
  tinykv::NewRandomAccessFile("test.bin", &acf);
  char buf[1024];
  tinykv::Slice result;
  tinykv::Status status = acf->Read(5, 1024, &result, buf);
  std::cout << status.ToString() << std::endl;
  std::cout << result.ToString() << std::endl;
  delete acf;

  return 0;
}
