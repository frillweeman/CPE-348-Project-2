// relative path to file of newline-delimited random numbers
#define RN_FILE_PATH "Project2_part1_rn.txt"
#define NUM_STATIONS 5

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const double TIME_SLOT_LEN = 51.2; // uS

// parallel arrays holding data for each station
int nextSlot[NUM_STATIONS] = {0, 0, 0, 0, 0};
int collisions[NUM_STATIONS] = {0, 0, 0, 0, 0};
bool hasTx[NUM_STATIONS] = {0, 0, 0, 0, 0};

// input file object
ifstream rnFile(RN_FILE_PATH);

// gets the next random number from the file
int getNextRandom();

// indicates whether or not all stations have transmitted
bool hasAllTx();

void print50Dashes();

int main()
{
  int slot = 0;
  do
  {
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
      }
    }
    else if (stationsTransmitting.size() == 1)
    {
      // First station to transmit (solution)
      int station = stationsTransmitting[0];
      printf("*  Station %d has successfully transmitted\n   in slot number %d, which is %.1f microseconds\n", station, slot, TIME_SLOT_LEN * slot);
      hasTx[station] = true;
      collisions[station] = 0; // reset collision count
      nextSlot[station] += 3;  // skip 2 slots before next packet tx attempt

      cout << "   current tx state [0:4]: ";
      printf("   current tx state [0:4]: [%d %d %d %d %d]\n\n", hasTx[0], hasTx[1], hasTx[2], hasTx[3], hasTx[4]);
    }

    ++slot;
  } while (!hasAllTx());
}

bool hasAllTx()
{
  for (auto tx : hasTx)
  {
    if (!tx)
      return false;
  }
  return true;
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