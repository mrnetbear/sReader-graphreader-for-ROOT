#include <iostream>
#include <math.h>
#include <ctime>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

int graph();
void cFileOpen(std::string lane, vector<double> &x, vector<double> &w);

//colors (for unix-systems only)
namespace Color {
    enum code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    class Modifier {
        code code;
    public:
        Modifier(enum code pCode) : code(pCode) {}
        friend std::ostream&
            operator << (std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

// openning file
void cFileOpen(string lane, vector<double> &x, vector<double> &w){
    Color::Modifier red(Color::FG_RED);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);

    ifstream fRead(lane);
    std::string clipboard;
    vector<string> list;
    int ln = 0;
    int beg = 0;

    //secifying data source
    cout << "Choose source: ";
    string src;
    cin >> src;
    if (src == "betaCN")
        beg = 65;
    if (src == "stdCN")
        beg = 65;
    if (src == "Excel")
        beg = 1;
    if (src == "lab1")
        beg = 0;

    //reading and printing file for checking
    while (getline(fRead, clipboard)){

        //filling clipboard
        list.push_back(clipboard);

        //busy flag for operator
        if (ln % 100 == 0){
            cout << ln << " busy..." << endl;
        }

        //file read for MEPHI CAEN
        if (src == "Excel" || src == "stdCN"){
            if (ln >= beg){
                int n;
                int q = 0;
                if (list[ln].find(' ') > list[ln].find('\t')){
                    n = list[ln].find('\t');
                    q = 1;
                }
                else
                    n = list[ln].find(' ');
                std::string xStr = (n != string::npos) ? list[ln].substr(0, n) : "";
                int n1 = list[ln].length();
                int n2 = n;
                while (n2 < n1) {
                    if (list[ln][n] == ' ')
                        n++;
                    n2++;
                }

                std::string yStr;
                if (q)
                    yStr = "0." + list[ln].substr(n+3, n1);
                else
                    yStr = list[ln].substr(n, n1);
                
                x.push_back(stof(xStr));
                w.push_back(stof(yStr));
                cout << "x = " << x[ln - beg] << "; w = " << w[ln - beg] << " xSize:" << x.size() << "; ySize: " << w.size() << endl;
            }
        }

        //file read for NRC KI CAEN
        else if (src == "betaCN"){
            if (ln >= beg){
                int n;
                n = list[ln].find('\t');
                //cout << n << endl;
                std::string xStr;
                int n0 = n;
                while (list[ln][n] != ' '){
                    n++;
                }
                xStr = list[ln].substr(n0, n);
                n0 = n;
                while (list[ln][n] != ' '){
                    n++;
                }
                std::string yStr;
                yStr = list[ln].substr(n0, n);
                x.push_back(stof(xStr));
                w.push_back(stof(yStr));
                // cout << "x = " << x[ln - beg] << "; w = " << w[ln - beg] << " xSize:" << x.size() << "; ySize: " << w.size() << endl;
            }
        }

        //file read for laboratory works
        else if (src == "lab1"){
            if (ln >= beg){
                cout << list[ln] << endl;
                //finding x-axis value
                int n = 0;
                while (list[ln][n] == ' '){
                    n++;
                }
                std::string xStr;
                int n0 = n;
                while (list[ln][n] != ' ' && list[ln][n] != '\t'){
                    n++;
                }
                xStr = list[ln].substr(n0, n);

                //finding y-axis value
                n = list[ln].length()-1;
                while (list[ln][n] != ' ' && list[ln][n] != '\t' && list[ln][n] != ','){
                    n--;
                }
                n0 = n;
                std::string yStr;
                if (src == "lab1")
                    yStr = list[ln].substr(n0, list[ln].length()-1);
                else
                    yStr = "0." + list[ln].substr(n0 + 1, list[ln].length()-1);

                cout << list[ln].substr(n0 + 1, list[ln].length()-1) << endl;
                cout << xStr << "; ";
                cout << yStr << endl;

                x.push_back(stof(xStr));
                w.push_back(stof(yStr));
            }
        }

        //error message
        else {
            cout << "Дурак, сломал всё" << endl;
            break;
        }
        ln++;
    }
    cout << clipboard << endl << endl << "---------------------" << endl << endl;
    cout << green << "Success!" << def << endl;
    fRead.close();
    cout << "---------------------" << endl;
}


int graph()
{
    Color::Modifier red(Color::FG_RED);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    cout << blue << "----------Graph Reader v. 2.0----------" << def << endl << "Created by A.D. Konotop" << endl << "Tested by:" << endl << "   A.D. Konotop" << endl << endl << "Released: 26 Oct 2023" << endl << green << "Last build: 26 Oct 2023" << def << endl;
    cout << endl << "Current time and date: " << asctime(timeinfo) << endl;
    cout << "---------------------------------------------" << endl << endl;
    while(true){
        std::string mode;
        cout << "Choose program mode" << endl << "> ";
        cin >> mode; //file path is requested here
        cout << "---------------------" << endl << endl;
        if (mode == ".q")
            break;
        else if(mode == "dhist"){
            while(true){
                std::string line;
                cout << "Enter the diff spect path in the line" << endl << "> ";
                cin >> line; //file path is requested here
                cout << "---------------------" << endl << endl;
                if (line == "back")
                    break;
                
                vector<double> x;
                vector<double> y;
                
                cFileOpen(line, x, y);
                
                //creating file name
                std::string rName;
                cout << "Create a file name: " << endl << "> ";
                cin >> rName;
                
                cout << "Size: " << x.size() << endl;
                TH1* h1 = new TH1F("Differential spectre", rName.c_str(), x.size(), x[0], x[x.size()-1]);

                for (int i = 0; i < x.size(); i++) {
                    for (int j = 0; j < y[i]; j++){
                        h1->Fill(x[i]);
                    }
                }
                
                gStyle->SetEndErrorSize(0);
                gStyle->SetErrorX(0);
                h1->SetMarkerStyle(20);
                
                rName = rName + ".root";
                TFile *oGrph = new TFile(rName.c_str(), "RECREATE");
                h1->Write();
                oGrph->Close();
                
                delete h1;
            }
        }
        else if (mode == "dtoi"){
            std::string line;
            cout << "Enter the diff spect path in the line" << endl << "> ";
            cin >> line; //file path is requested here
            cout << "---------------------" << endl << endl;

            vector<double> difX;
            vector<double> difY;

            cFileOpen(line, difX, difY);

            cout << "Size: " << difX.size() << endl;

            cout << "Enter the integ spect path in the line" << endl << "> ";
            cin >> line; //file path is requested here
            cout << "---------------------" << endl << endl;

            vector<double> integX;
            vector<double> integY;

            cFileOpen(line, integX, integY);

            for (int i = 0; i < integX.size(); i++){
                integX[i] = -integX[i];
            }

            //creating file name
            std::string rName;
            cout << "Create a file name: " << endl << "> ";
            cin >> rName;

            TH1* diff = new TH1F("Differential spectre", rName.c_str(), difX.size(), difX[0], difX[difX.size()-1]);
            for (int i = 0; i < difX.size(); i++) {
                for (int j = 0; j < difY[i]; j++){
                    diff->Fill(difX[i]);
                }
            }

            TGraph *integ = new TGraph(integX.size(), integX.data(), integY.data());

            integ -> SetMarkerStyle(21);

            vector<double> dtoiX;
            vector<double> dtoiY;

            for (int i = 0; i < difX.size(); i++){
                double sum = 0;
                dtoiX.push_back(difX[i]);
                for (int j = i; j < difX.size(); j++){
                    sum += difY[j];
                }
                dtoiY.push_back(sum);
            }

            TGraph *grdtoi = new TGraph(dtoiX.size(), dtoiX.data(), dtoiY.data());

            grdtoi -> SetMarkerStyle(21); 

            vector<double> itodX;
            vector<double> itodY;

            for (int i = 0; i < integX.size(); i++){
                itodX.push_back(integX[i]);
                double d = 0;
                if (i == 0){
                    d = (integY[i+1] - integY[i])/(integX[i+1] - integX[i]);
                }
                if (i == (integX.size() - 1)){
                    d = (integY[i] - integY[i-1])/(integX[i] - integX[i-1]);
                }
                else{
                    d = ((integY[i+1] - integY[i])/(integX[i+1] - integX[i]) + (integY[i] - integY[i-1])/(integX[i] - integX[i-1]) + (integY[i+1] - integY[i-1])/(integX[i+1] - integX[i-1])) / 3;
                }
                d = abs(d) * 1000;
                itodY.push_back(d);
            }

            cout << itodX.size() << endl;

            TH1* hitod = new TH1F("Integral to differential spectre", rName.c_str(), itodX.size(), itodX[0], itodX[itodX.size()-1]);
            for (int i = 0; i < itodX.size(); i++) {
                for (int j = 0; j < itodY[i]; j++){
                    hitod->Fill(itodX[i]);
                }
            }

            rName = rName + ".root";
            TFile *oGrph = new TFile(rName.c_str(), "RECREATE");

            diff->Write();
            grdtoi->Write();
            integ->Write();
            hitod->Write();
            oGrph->Close();

            delete diff;
            delete grdtoi;
            delete integ;
            delete hitod;
        }
    }
    cout << "Bye! :)" << endl;
    return 0;
}
