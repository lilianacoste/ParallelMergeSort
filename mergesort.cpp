#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <thread>

#define DEBUG 0
const int THRESHOLD = 1000;

void generateMergeSortData(std::vector<int>& arr, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    arr[i] = rand();
  }
}

void checkMergeSortResult(std::vector<int>& arr, size_t n) {
  bool ok = true;
  for (size_t i = 1; i < n; ++i) {
    if (arr[i] < arr[i - 1]) {
      ok = false;
      break;
    }
  }
  if (!ok)
    std::cerr << "notok" << std::endl;
}

void merge(int* arr, size_t l, size_t mid, size_t r, int* temp) {
#if DEBUG
  std::cout << l << " " << mid << " " << r << std::endl;
#endif

  if (l == r) return;
  if (r - l == 1) {
    if (arr[l] > arr[r]) {
      std::swap(arr[l], arr[r]);
    }
    return;
  }

  size_t i, j, k;
  size_t n = mid - l;

  // Copy left half to temp
  for (i = 0; i < n; ++i)
    temp[l + i] = arr[l + i];

  i = 0;         // index into temp
  j = mid;       // index into right half
  k = l;         // index into result array

  while (i < n && j <= r) {
    if (temp[l + i] <= arr[j]) {
      arr[k++] = temp[l + i++];
    } else {
      arr[k++] = arr[j++];
    }
  }

  // Copy remaining from temp (left side)
  while (i < n) {
    arr[k++] = temp[l + i++];
  }
}

void mergesort(int* arr, size_t l, size_t r, int* temp) {
  if (l < r) {
    size_t mid = (l + r) / 2;
    mergesort(arr, l, mid, temp);
    mergesort(arr, mid + 1, r, temp);
    merge(arr, l, mid + 1, r, temp);
  }
}

void mergeSortParallel(std::vector<int>& arr, int left, int right, std::vector<int>& temp) {
  if (left < right) {
    if ((right - left + 1) < THRESHOLD) {
      mergesort(arr.data(), left, right, temp.data());
    } else {
      int mid = (left + right) / 2;
      std::thread t1(mergeSortParallel, std::ref(arr), left, mid, std::ref(temp));
      std::thread t2(mergeSortParallel, std::ref(arr), mid + 1, right, std::ref(temp));
      t1.join();
      t2.join();
      merge(arr.data(), left, mid + 1, right, temp.data());
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
    return -1;
  }

  size_t n = atol(argv[1]);

  // Generate same input for both
  std::vector<int> original(n);
  generateMergeSortData(original, n);
  
  std::vector<int> arr_seq = original;
  std::vector<int> arr_par = original;
  std::vector<int> temp(n);

  // Time Sequential
  auto start_seq = std::chrono::high_resolution_clock::now();
  mergesort(arr_seq.data(), 0, n - 1, temp.data());
  auto end_seq = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_seq = end_seq - start_seq;
  checkMergeSortResult(arr_seq, n);

  // Time Parallel
  auto start_par = std::chrono::high_resolution_clock::now();
  mergeSortParallel(arr_par, 0, n - 1, temp);
  auto end_par = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_par = end_par - start_par;
  checkMergeSortResult(arr_par, n);

  // Output as CSV
  std::cout << "n,seq_time,par_time\n";
  std::cout << n << "," << time_seq.count() << "," << time_par.count() << std::endl;

  return 0;
}

