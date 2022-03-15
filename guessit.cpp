#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

int generateRandomNumber() {
    return rand() % 100 + 1;
}

int getPlayerGuess() {
    int guess;
    cout << "Enter your guess (1...100): ";
    cin >> guess;
    return guess;
}

void printAnswer(int guess, int secretNumber) {
    if (guess == secretNumber) {
        cout << "Congratulation! You win.\n";
    }
    else if (guess < secretNumber) {
        cout << "Your number is smaller than the secret number.\n";
    }
    else {
        cout << "Your number is bigger than the secret number.\n";
    }
}

int main()
{
    ios::sync_with_stdio();
    cin.tie(0);
    srand(time(NULL));
    int secretNumber = generateRandomNumber();
    bool running  = true;
    while (running) {
        int playerGuess = getPlayerGuess();
        printAnswer(playerGuess, secretNumber);
        if (playerGuess == secretNumber) {
            running = false;
        }
    }
    return 0;
}
