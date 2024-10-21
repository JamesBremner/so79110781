#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "cxy.h"

struct sline
{
    int y;
    int x1;
    int x2;
};

std::vector<sline> theLines;
std::pair<cxy, cxy> theVert;

void genRand(int lineCount, int maxX)
{
    srand(time(NULL));
    for (int kl = 1; kl < lineCount; kl++)
    {
        sline l;
        l.y = kl;
        l.x1 = rand() % maxX;
        l.x2 = l.x1 + rand() % (maxX / 2) + 1;
        theLines.push_back(l);
    }
}

std::pair<cxy, cxy> algo()
{
    std::pair<cxy, cxy> ret = std::make_pair(cxy(0, 0), cxy(0, 0));

    std::sort(
        theLines.begin(), theLines.end(),
        [](const sline &a, const sline &b)
        {
            return a.y < b.y;
        });

    for (auto &l : theLines)
    {
        for (int x = l.x1; x <= l.x2; x++)
        {
            cxy le1(x, l.y);
            cxy le2(x, 11);
            for (auto &l2 : theLines)
            {
                if (l2.y <= l.y)
                    continue;
                cxy l2e1(l2.x1, l2.y);
                cxy l2e2(l2.x2, l2.y);
                // std::cout << le1 << " to " << le2 << " against "
                //           << l2e1 << " to " << l2e2 << "\n";
                cxy p;
                if (cxy::isIntersection(
                        p, le1, le2, l2e1, l2e2))
                    break;
                if (l2.y == 9)
                {
                    ret = std::make_pair(
                        le1, le2);
                    return ret;
                }
            }
        }
    }
    return ret;
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500})
    {
        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                for (auto &l : theLines)
                    S.line({10 * l.x1, 10 * l.y, 10 * l.x2, 10 * l.y});
                S.color(0xFF, 0, 0);
                S.line({10 * theVert.first.x, 10 * theVert.first.y,
                        10 * theVert.second.x, 10 * theVert.second.y});
            });
        show();
        run();
    }

private:
};

main()
{
    genRand(10, 100);
    theVert = algo();
    cGUI theGUI;
    return 0;
}
