#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> foodQueue;

const int maxFood = 10;
bool isMakerDone = false;

void maker() {
    for (int i = 1; i <= maxFood; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            foodQueue.push(i);
            std::cout << "Made: " << i << std::endl;
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    {//Finish making
        std::lock_guard<std::mutex> lock(mtx);
        isMakerDone = true;
    }
    cv.notify_one();
}

void eater() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !foodQueue.empty() || isMakerDone; });
        if (foodQueue.empty() && isMakerDone) {
            std::cout << "Eater exit" << std::endl;
            break;
        }

        int food = foodQueue.front();
        foodQueue.pop();
        lock.unlock();

        std::cout << "Eaten: " << food << std::endl;
    }
}

int main() {
    std::thread makerThread(maker);
    std::thread eaterThread(eater);

    makerThread.join();
    eaterThread.join();

    return 0;
}
