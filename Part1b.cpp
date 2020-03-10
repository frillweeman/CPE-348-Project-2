// relative path to file of newline-delimited random numbers
#define RN_FILE_PATH "Project2_part1_rn.txt"
#define NUM_STATIONS 6

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const double TIME_SLOT_LEN = 51.2; // uS

// parallel arrays holding data for each station
int nextSlot[NUM_STATIONS] = {0, 0, 0, 0, 0, 0};
int collisions[NUM_STATIONS] = {0, 0, 0, 0, 0, 0};

// input file object
ifstream rnFile(RN_FILE_PATH);

// gets the next random number from the file
int getNextRandom();

// gets max value of the nextSlot array
// used for exiting loop
int getMaxSlot();

void print50Dashes();

int main()
{
  int slot = 0;
  do
  {
    print50Dashes();
    cout << "Checking time slot: " << slot << endl;

    vector<int> stationsTransmitting;

    // collect a list of stations transmitting
    for (int station = 0; station < NUM_STATIONS; ++station)
    {
      if (nextSlot[station] == slot)
      {
        stationsTransmitting.push_back(station);
      }
    }

    // iterate over list of stations transmitting
    if (stationsTransmitting.size() > 1)
    {
      for (int station : stationsTransmitting)
      {
        // add to collision count for this station
        ++collisions[station];

        // calculate next slot based on current number of collisions and random number
        nextSlot[station] += (getNextRandom() % (int)pow(2.0, collisions[station])) + 1;

        printf("-  Station %d: backing off to slot %d  collisions: %d\n", station, nextSlot[station], collisions[station]);
      }
    }
    else if (stationsTransmitting.size() == 1)
    {
      // First station to transmit (solution)
      printf("*  Station %d has successfully transmitted\n   in slot number %d, which is %.1f microseconds\n", stationsTransmitting[0], slot, TIME_SLOT_LEN * slot);
      // break;
    }
    else
    {
      // no stations transmit ("state that fact")
      printf("X  no stations have transmitted in this slot\n");
    }

    print50Dashes();
    cout << "\n\n";

    ++slot;
  } while (getMaxSlot() >= slot);
}

int getMaxSlot()
{
  int maxVal = 0;
  for (auto s : nextSlot)
  {
    if (s > maxVal)
      maxVal = s;
  }
  return maxVal;
}

int getNextRandom()
{
  int currentNum;
  if (rnFile >> currentNum)
    return currentNum;
  return 0; // false if EOF
}

void print50Dashes()
{
  for (int i = 0; i < 50; ++i)
    cout << '-';

  cout << endl;
}