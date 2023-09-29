
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

void cFileOpen(string lane, vector<double> &x, vector<double> &w){
    Color::Modifier red(Color::FG_RED);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    ifstream fRead(lane);
    char clipboard[250];
    vector<string> list;
    int ln = 0;
    int beg = 0;
    cout << "Choose source: "; //Вбираем источник данных
    string src;
    cin >> src;
    if (src == "betaCN")
        beg = 65;
    if (src == "stdCN")
        beg = 65;
    if (src == "Excel")
        beg = 0;
    if (src == "lab1")
        beg = 0;
    while (fRead.getline(clipboard, 250)){ // считываем и выводим файл для проверки (reading and printing file for checking)
        list.push_back(clipboard);             //считыаем значения по x и y (getting values of x & y)
        if (ln % 100 == 0)
            cout << ln << " busy..." << endl;
        //cout << list[ln] << endl;
        //cout << list[ln] << "---|---" << list.size() << endl;         // x собираем до пробела, y - после (getting x before space, y - after spaces)
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
                
                //cout << "xStr = " << xStr << "; yStr = " << yStr << endl;
                x.push_back(stof(xStr));
                w.push_back(stof(yStr));
                cout << "x = " << x[ln - beg] << "; w = " << w[ln - beg] << " xSize:" << x.size() << "; ySize: " << w.size() << endl;
            }
        }
        else if (src == "betaCN"){
            if (ln >= beg){
                int n;
                n = list[ln].find('\t');
                cout << n << endl;
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
                cout << "x = " << x[ln - beg] << "; w = " << w[ln - beg] << " xSize:" << x.size() << "; ySize: " << w.size() << endl;
            }
        }
        else if (src == "lab1"){
            if (ln >= beg){
                int n = 0;
                while (list[ln][n] == ' '){
                    n++;
                }
                //cout << n << endl;
                std::string xStr;
                int n0 = n;
                while (list[ln][n] != ' '){
                    n++;
                }
                xStr = list[ln].substr(n0, n-1);
                while (list[ln][n] == ' '){
                    n++;
                }
                n0 = n;

                std::string yStr;
                yStr = list[ln].substr(n0, 19);
                x.push_back(stof(xStr));
                w.push_back(stof(yStr));
                //cout << "x = " << x[ln - beg] << "; w = " << w[ln - beg] << " xSize:" << x.size() << "; ySize: " << w.size() << endl;
            }
        }
        else {
            cout << "Дурак, сломал всё" << endl;
            break;
        }
        ln++;
    };
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
    cout << blue << "----------Graph Reader v. 1.0----------" << def << endl << "Created by A.D. Konotop" << endl << "Tested by:" << endl << "   A.D. Konotop (Mac)" << endl << endl << "Released: 14 Mar 2023" << endl << green << "Last build: 14 Mar 2023" << def << endl;
    cout << endl << "Current time and date: " << asctime(timeinfo) << endl;
    cout << "---------------------------------------------" << endl << endl;

    std::string line;
    cout << "Enter the diff spect path in the line" << endl << "> ";
    cin >> line; // здесь вводится путь файла (file path is requested here)
    //cout << "Writing..." << endl;
    //ofstream write;
    char clipboard[250]{}; // вводится "рабочая строка" ("working" string is introduced)
    //ifstream read(lane);
    cout << "---------------------" << endl << endl;
    std::string list[2500];
    int ln = 0;
    
    vector<double> x;
    vector<double> y;
    
    cFileOpen(line, x, y);
    
    std::string rName;
    cout << "Create a file name: " << endl << "> ";
    cin >> rName;
    
    cout << "Size: " << x.size() << endl;
    TH1* h1 = new TH1F("h1", rName.c_str(), x.size(), x[0], x[x.size()-1]);

    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < y[i]; j++){
            //cout << "Busy!" << endl;
            h1->Fill(x[i]);
        }
        //cout << "Done! Next..." << endl;
    }
    
    gStyle->SetEndErrorSize(0);
    gStyle->SetErrorX(0);
    h1->SetMarkerStyle(20);
    
    rName = rName + ".root";
    TFile *oGrph = new TFile(rName.c_str(), "RECREATE");
    h1->Write();
    //h2->Write();
    oGrph->Close();
    
    h1->Draw("");
    //h2->Draw();
    //h1->Delete();
    
    //delete h1;
    return 0;
}
