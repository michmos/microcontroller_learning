#include <EEPROM.h>
#include <stdlib.h>

#define LEADERBOARD_SIZE 16
#define EEPROM_SIZE 1024

// the last two bytes of EEPROM are used to store an identifier that indicates
// whether EEPROM is setup correctly for this program. If not it will be setup
#define IDENTIFIER 0xABCD
#define IDENTIFIER_ADDR (EEPROM_SIZE - 2)

// the third and fourth last bytes are used to store the EEPROM address of the
// current leaderboard (a leaderboard always tracks LEADERBOARD_SIZE places, before
// a new one is created in the memory aread next to it)
#define EEPROM_LEADERBOARD_START_ADDR (EEPROM_SIZE - 4)

// returns the actual EEPROM addr for a leaderboard idx between 0 - 15
#define EEPROM_SHIFTED_IDX(leaderboardIdx) (g_eeprom_leaderboard_start_idx + leaderboardIdx)

// eeprom index where current leaderboard starts
// uint16_t eepromLeaderboardStartIdx;
uint16_t g_eeprom_leaderboard_start_idx;

int compare(const void* a, const void* b) {
  return (*(uint8_t*)a - *(uint8_t*)b);
}

class Leaderboard {
private:
  // leaderboards are only sorted in cache (not in the actual EEPROM) to avoid
  // frequent rewrites
  uint8_t sortedCache[LEADERBOARD_SIZE];
  uint8_t length_;

public:
  Leaderboard() : length_(0) {};

  void init() {
    // setup eeprom
    uint16_t id;
    EEPROM.get(IDENTIFIER_ADDR, id);
    if (id != IDENTIFIER) {
      resetLeaderboard();
      EEPROM.put(IDENTIFIER_ADDR, IDENTIFIER);
      EEPROM.put(EEPROM_LEADERBOARD_START_ADDR, 0);
    }

    // get current eeprom region idx
    EEPROM.get(EEPROM_LEADERBOARD_START_ADDR, g_eeprom_leaderboard_start_idx);

    // get current leaderboard length and setup sortedCacheLeaderboard
    length_ = 0;
    while (length_ < LEADERBOARD_SIZE && EEPROM[EEPROM_SHIFTED_IDX(length_)] != 0) {
      sortedCache[length_] = EEPROM.read(EEPROM_SHIFTED_IDX(length_));
      length_++;
    }
    if (length_ == LEADERBOARD_SIZE) {
      resetLeaderboard();
    }
  }

  void print(uint8_t countToBeMarked) {
    bool printedCountMark = false;

    Serial.println("---------------");
    Serial.println("| LEADERBOARD |");
    Serial.println("---------------");

    for (uint8_t i = 0; i < length_; ++i) {
      Serial.print(i + 1);
      Serial.print(". : ");
      int8_t guesses = sortedCache[i];
      Serial.print(guesses);
      Serial.print(" Guesses");
      if (guesses == countToBeMarked && !printedCountMark) {
        Serial.print(" <-- you");
        printedCountMark = true;
      }
      Serial.println();
    }
  }

  void add(uint8_t count) {
    if (length_ == LEADERBOARD_SIZE) {
      resetLeaderboard();
    }

    // safe to EEPROM
    EEPROM.update(EEPROM_SHIFTED_IDX(length_), count);
    // safe to cache
    sortedCache[length_] = count;

    length_++;

    // resort (IMPROVE: replace by insertion sort)
    qsort(sortedCache, length_, sizeof(uint8_t), compare);
  }

  // instead of resetting the actual eeprom cells, we just
  // move forwards to the next cells in eeprom for wear leveling purposes
  void resetLeaderboard() {
    // update EEPROM region that is being written to
    g_eeprom_leaderboard_start_idx = EEPROM_SHIFTED_IDX(LEADERBOARD_SIZE);
    if (g_eeprom_leaderboard_start_idx + LEADERBOARD_SIZE - 1 >= EEPROM_SIZE - 4) {
      g_eeprom_leaderboard_start_idx = 0;
    }
    EEPROM.put(EEPROM_LEADERBOARD_START_ADDR, g_eeprom_leaderboard_start_idx);

    // 0 init arrays
    for (uint8_t i = 0; i < LEADERBOARD_SIZE; ++i) {
      sortedCache[i] = 0;
      EEPROM.write(EEPROM_SHIFTED_IDX(i), 0);
    }
    length_ = 0;
  }
};

struct s_game {
  uint8_t numberToGuess;
  uint8_t count;
};

struct s_game currentGame;
struct Leaderboard leaderboard;

void resetGame() {
  randomSeed(analogRead(0));
  currentGame.numberToGuess = random(0, 101);

  currentGame.count = 1;

  Serial.println("Guess a number between 0 - 100");
}

int getInput() {
  char buffer[16] = { '\0' };
  int8_t bytesRead = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
  buffer[bytesRead] = '\0';

  int8_t number = atoi(buffer);
  if ((number == 0 && !(buffer[0] == '0' && bytesRead == 1))
      || (number < 0 || number > 100)) {
    return (-1);
  }
  return (number);
}


void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to the number guessing game");

  leaderboard.init();
  resetGame();
}

void loop() {
  if (Serial.available()) {
    int8_t numberGuessed = getInput();
    if (numberGuessed == -1) {
      Serial.println("Please provide a number in the following range: 0 - 100");
      return;
    }

    if (numberGuessed < currentGame.numberToGuess) {
      Serial.println("Too low!");
      currentGame.count++;
    } else if (numberGuessed > currentGame.numberToGuess) {
      Serial.println("Too high!");
      currentGame.count++;
    } else {
      Serial.println("Correct. Congrats!");
      leaderboard.add(currentGame.count);
      leaderboard.print(currentGame.count);
      resetGame();
    }
  }
}
