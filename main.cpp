#include <iostream>
#include "generator.h"
#include "rein.h"
#include "tama.h"
#include "enmatp.h"
#include "enmatp_fg.h"


using namespace std;
int main(int argc, char **argv)
{
    int subs;           // Number of subscriptions.
    int pubs;           // Number of publications.
    int atts;           // Total number of attributes, i.e. dimensions.
    int cons;           // Number of constraints(predicates) in one sub.
    int m;              // Number of constraints in one pub.
    int valDom;         // Cardinality of values.
    double width;       // Width of a predicate.
    int buck_num;
    int tama_level;
    int update_window;
    ifstream parafile;
    parafile.open(string("./paras/paras_1.txt"));
    parafile >> subs >> pubs >> atts >> cons >> m >> valDom >> width >> buck_num >> tama_level >> update_window;
    parafile.close();
    m = atts;
    vector<double> comatTimeList, reinTimeList, tamaTimeList, enmatTimeList[5], enmatpTimeList[5], enmatpfrTimeList[5];
    vector<double> adaReinTimeList, adaReinSubList;
    vector<double> opIndexTimeList, opIndexSubList;
    // Initiate generator
    intervalGenerator gen(subs, pubs, atts, cons, m, valDom, width);
    gen.GenSubList();
    gen.GenPubList();
//    gen.ReadPubList();
//    gen.ReadSubList();

    cout<<"gen complete"<<endl;
    string outputFileName = "";
    string content = "";
    Rein rein(valDom, buck_num);
    Tama tama(atts, valDom, tama_level);
    Enmatp enmatp(valDom, buck_num, atts, tama_level, false);
    Enmatpfr enmatpfr(valDom, buck_num, atts, tama_level, false);

    vector<IntervalSub> TamasubList;

    cout<<"init complete"<<endl;

    int cursub = 0, curpub = 0;
    const int subwindow = 100, pubwindow = 10;
    while (cursub < subs){
        for (int i = 0; i < subwindow; i++)
        {
            const IntervalSub &sub = gen.subList[cursub+i];
            rein.insert(sub);
            tama.insert(sub);
            enmatp.insert(sub);
            enmatpfr.insert(sub, TamasubList);
        }
        cursub += subwindow;

        for (int i = 0; i < pubwindow; ++i){
            const Pub &pub = gen.pubList[i];
            int matchSubs = 0;

            Timer matchStart;
            tama.match(pub, matchSubs, gen.subList);
            double tamaTime = (double)matchStart.elapsed_nano() / 1000000;
            tamaTimeList.push_back(tamaTime);

            matchStart.reset();
            rein.match(pub, matchSubs);
            double reinTime = (double)matchStart.elapsed_nano() / 1000000;
            reinTimeList.push_back(reinTime);

            int matchSubs1 = 0, matchSubs2 = 0;
            int64_t t1 = 0, t2 = 0;
            matchStart.reset();
            enmatp.match(pub, gen.subList, matchSubs1, matchSubs2, t1, t2);
            double enmatpTime = (double)matchStart.elapsed_nano() / 1000000;
            enmatpTimeList[0].push_back(enmatpTime);
            enmatpTimeList[1].push_back((double)t1 / 1000000);
            enmatpTimeList[2].push_back((double)t2 / 1000000);
            enmatpTimeList[3].push_back(matchSubs1);
            enmatpTimeList[4].push_back(matchSubs2);

            matchSubs1 = matchSubs2 = 0;
            t1 = t2 = 0;
            matchStart.reset();
            enmatpfr.match(pub, TamasubList, matchSubs1, matchSubs2, t1, t2);
            double enmatpfrTime = (double)matchStart.elapsed_nano() / 1000000;
            enmatpfrTimeList[0].push_back(enmatpfrTime);
            enmatpfrTimeList[1].push_back((double)t1 / 1000000);
            enmatpfrTimeList[2].push_back((double)t2 / 1000000);
            enmatpfrTimeList[3].push_back(matchSubs1);
            enmatpfrTimeList[4].push_back(matchSubs2);
        }
        curpub += pubwindow;
        cout << "match complete "<< curpub <<endl;

        if (!(curpub % update_window))
            cout<< "update split point: " << enmatpfr.adjustsplit() << "    " << enmatp.adjustsplit() << endl;


        if((cursub % 1000) == 0){
                    content =   Util::Int2String(cursub) + "\t" +
                        Util::Double2String(reinTimeList.back());
            outputFileName = "./result/rein.txt";
            Util::WriteData(outputFileName.c_str(), content);

            content =   Util::Int2String(cursub) + "\t" +
                        Util::Double2String(tamaTimeList.back());
            outputFileName = "./result/tama.txt";
            Util::WriteData(outputFileName.c_str(), content);

            content =   Util::Int2String(cursub) + "\t" +
                        Util::Double2String(enmatTimeList[0].back()) + "\t" +
                        Util::Double2String(enmatTimeList[1].back()) + "\t" +
                        Util::Double2String(enmatTimeList[2].back()) + "\t" +
                        Util::Double2String(enmatTimeList[3].back()) + "\t" +
                        Util::Double2String(enmatTimeList[4].back());
            outputFileName = "./result/enmat.txt";
            Util::WriteData(outputFileName.c_str(), content);

            content =   Util::Int2String(cursub) + "\t" +
                        Util::Double2String(enmatpTimeList[0].back())  + "\t" +
                        Util::Double2String(enmatpTimeList[1].back())  + "\t" +
                        Util::Double2String(enmatpTimeList[2].back())  + "\t" +
                        Util::Double2String(enmatpTimeList[3].back())  + "\t" +
                        Util::Double2String(enmatpTimeList[4].back());
            outputFileName = "./result/enmatp.txt";
            Util::WriteData(outputFileName.c_str(), content);

            content =   Util::Int2String(cursub) + "\t" +
                        Util::Double2String(enmatpfrTimeList[0].back())  + "\t" +
                        Util::Double2String(enmatpfrTimeList[1].back())  + "\t" +
                        Util::Double2String(enmatpfrTimeList[2].back())  + "\t" +
                        Util::Double2String(enmatpfrTimeList[3].back())  + "\t" +
                        Util::Double2String(enmatpfrTimeList[4].back());
            outputFileName = "./result/enmatpfr.txt";
            Util::WriteData(outputFileName.c_str(), content);
        }
    }
    return 0;
}
;