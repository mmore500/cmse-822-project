#ifndef __CELL_H__
#define __CELL_H__

class Cell : public CBase_Cell {

private:

  CProxySection_Tile tileNeighbors;
  CProxySection_Cell cellNeighbors;

  /* Who am I? */
  size_t channelID;
  size_t x;
  size_t y;

  /* Cached environmental information */
  double stockpiles[Cardi.NumDirs];
  size_t channelIDs[Cardi.NumDirs];

  /* Organism state */
  //bool mutes[Cardi.NumDirs]; // who do I choose to ignore signals from?
  //size_t facing;

public:

  /* Constructors */
  Cell();
  Cell(CkMigrateMessage *msg);

  /* Entry Methods */
  void updateStockpile(double amount, size_t neighbor);
  void updateChannelID(size_t channelID, size_t neighbor);

};

#endif //__HELLO_H__
