#include <thread>
#include <iostream>
#include <mutex>

// Мьютекс, который нужен чтобы потоки выполнялись поочередно.
std::mutex mutex;

// Ввод данных.
void input(int &m, int &n, int &time_first, int &time_second) {
  std::cout << "Count of rows:";
  std::cin >> m;
  std::cout << "Count of columns:";
  std::cin >> n;
  std::cout << "Enter the execution time for the first and second gardener separated by a space:";
  std::cin >> time_first >> time_second;
}

// Заполнение сада.
int **fillGarden(int m, int n) {
  int **array = new int *[m];
  for (int i = 0; i < m; i++) {
    array[i] = new int[n];
  }
  for (int count_row = 0; count_row < m; count_row++) {
    for (int count_column = 0; count_column < n; count_column++) {
      array[count_row][count_column] = 3;
    }
  }
  return array;
}

// Удаление из памяти.
void deleteGarden(int m, int *const *array) {
  for (int count = 0; count < m; count++) {
    delete[] array[count];
  }
  delete[] array;
}

// Отображает работу садовников.
void printWork(int gardner) {
  mutex.lock();
  if (gardner == 1) {
    std::cout << "The first gardener processed the plot " << std::endl;
  } else {
    std::cout << "The second gardener processed the plot " << std::endl;
  }
  mutex.unlock();
}

// Отображение состояния сада.
void printArray(int m, int n, int *const *array) {
  mutex.lock();
  std::cout << std::endl;
  for (int count_row = 0; count_row < m; count_row++) {
    for (int count_column = 0; count_column < n; count_column++) {
      std::cout << array[count_row][count_column] << "   ";
    }
    std::cout << std::endl;
  }
  mutex.unlock();
}

// Симулирует работу первого садовника.
void firstGardner(int m, int n, int **array, int time) {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (array[i][j] == 3) {
        array[i][j] = 1;
        std::this_thread::sleep_for(std::chrono::seconds(time));
        printWork(1);
        printArray(m, n, array);
        std::cout << std::endl;
      } else {
        continue;
      }
    }
  }
}

// Симулирует работу второго садовника.
void secondGardner(int m, int n, int **array, int time) {
  for (int i = n - 1; i >= 0; --i) {
    for (int j = m - 1; j >= 0; --j) {
      if (array[j][i] == 3) {
        array[j][i] = 2;
        std::this_thread::sleep_for(std::chrono::seconds(time));
        printWork(2);
        printArray(m, n, array);
        std::cout << std::endl;
      } else {
        continue;
      }
    }
  }
}

// Инциализация садовников (потоков).
void initThread(int n, int time_first, int time_second, int &m, int **&array) {
  std::thread first_gardner(firstGardner, m, n, array, time_first);
  std::thread second_gardner(secondGardner, m, n, array, time_second);
  second_gardner.join();
  first_gardner.join();
}

int main() {
  try {
    auto start_time = std::chrono::high_resolution_clock::now();
    int m, n, time_first, time_second;
    input(m, n, time_first, time_second);
    std::cout
        << "\n=================================================Start program==============================================="
        << std::endl;
    int **array = fillGarden(m, n);
    initThread(n, time_first, time_second, m, array);
    deleteGarden(m, array);
    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout
        << "\n================================================Finish program===============================================\n"
        << std::endl;
    std::cout << "Time spent: " << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count()
              << " seconds" << std::endl;
    system("pause");
  } catch (const std::bad_array_new_length &e) {
    std::cout << "Incorrect input\n";
    return 1;
  } catch (std::exception &e) {
    std::cerr << "Something wrong: " << e.what() << '\n';
    return 2;
  }
  return 0;
}

