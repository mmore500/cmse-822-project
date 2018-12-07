#ifndef __MAIN_H__
#define __MAIN_H__

class Main : public CBase_Main {

  private:

    /* Member Variables */
    // aka Object State
    size_t doneCount;
    size_t numElements;
    double* result;
    int* updateCounts;

  public:

    /* Constructors */
    Main(CkArgMsg* msg);
    Main(CkMigrateMessage* msg);

    /* Entry Methods */
    void done(size_t which, double amount, int updateCount);

};

#endif //__MAIN_H__
