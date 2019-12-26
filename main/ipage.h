#ifndef PAGE_I
#define PAGE_I

class IPage {
  public:
    char TextB[8] = "non";
    char TextC[8] = "non";

    virtual void start ();
    virtual void update();
    virtual void onPressedB();
    virtual void onPressedC();
};

#endif
