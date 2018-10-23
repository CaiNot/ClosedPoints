#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

int square(int x) {
    return x * x;
}

bool cmpX(int *p, int *q) {
    if (p[0] == q[0]) {
        return p[1] < q[1];
    } else
        return p[0] < q[0];
}

bool cmpY(int *p, int *q) {
    if (p[1] == q[1]) {
        return p[0] < q[0];
    } else
        return p[1] < q[1];
}

class Solution {
private:
    int pointsNum; // 总共的点数
    vector<int *> points;
    vector<int *> pointsX; // 以X为基准 升序
    vector<int *> pointsY; // 以Y为基准 升序
    int minDis; // 保存最短距离
    int point_1[2], point_2[2]; // 保存最近点对

public :
    Solution() : pointsNum(0), points(0) {
        this->minDis = INT_MAX;
        this->point_1[0] = -1;
        this->point_1[1] = -1;
        this->point_2[0] = -1;
        this->point_2[1] = -1;
    }

    bool readFromFile(string filePath) {
//        int pointsNumFromFile = 0;
        int x, y;
        string line;
        ifstream infile(filePath);
        stringstream ss;

        if (!infile.is_open()) {
            cout << "File Open Error" << endl;
            return false;
        }

//        while (getline(infile, line)) {
//            pointsNumFromFile++;
//        }

//        this->points = new vector<[pointsNumFromFile]();
//        for (int i = 0; i < pointsNumFromFile; i++) {
//            this->points[i] = new int[2]();
//        }

//        infile.clear();
//        infile.seekg(0);
        int pos = 0;
        int *posTemp = 0;
        while (!infile.eof()) {
            getline(infile, line);
            ss.clear();
            ss.str(line);
            ss >> x >> y;
            if (ss.fail()) {
                continue;
            }
            posTemp = new int[2];
            posTemp[0] = x;
            posTemp[1] = y;
            this->points.push_back(posTemp);
            
            pos++;
            cout << "x: " << x << " y: " << y << endl;
        }
        this->pointsNum = pos;
        cout << "the num of points is " << pos << endl;

        infile.close();
        return true;
    }

    void solveByCommon() {
        int disTemp = 0;
        for (int i = 0; i < this->pointsNum; i++) {
            for (int j = i + 1; j < this->pointsNum; j++) {
                disTemp = square(this->points[i][0] - this->points[j][0]) +
                          square(this->points[i][1] - this->points[j][1]);
                if (disTemp < this->minDis) {
                    this->minDis = disTemp;
                    this->point_1[0] = this->points[i][0];
                    this->point_1[1] = this->points[i][1];
                    this->point_2[0] = this->points[j][0];
                    this->point_2[1] = this->points[j][1];
                }
            }
        }
    }

    void sortByX() {
        sort(this->points.begin(), this->points.end(), cmpX);
        this->pointsX.assign(this->points.begin(), this->points.end());

    }

    void sortByY() {
        sort(this->points.begin(), this->points.end(), cmpY);
        this->pointsY.assign(this->points.begin(), this->points.end());

    }

    void copyPoints(int **&source, int **&backUp) {
        backUp = new int *[this->pointsNum];
        for (int i = 0; i < this->pointsNum; i++) {
            backUp[i] = new int[2];
            backUp[i][0] = source[i][0];
            backUp[i][1] = source[i][1];
        }
    }

    void solveByDivider() {
        this->sortByX();
        this->sortByY();
    }

    void printResult() {
        cout << "the closest pair of points in " << this->pointsNum << " points is:" << endl;
        cout << "Point_1:\n\tx: " << this->point_1[0] << " y: " << this->point_1[1] << endl;
        cout << "Point_2:\n\tx: " << this->point_2[0] << " y: " << this->point_2[1] << endl;
        cout << "the distance's square between two points is " << this->minDis << endl;
    }

    void printPoints() {
        cout << "All Points: " << endl;
        for (int i = 0; i < this->pointsNum; i++) {
            cout << "x: " << this->points[i][0] << " y: " << this->points[i][1] << endl;
        }
    }

    ~Solution() {
        for (int i = 0; i < this->points.size(); i++) {
            delete[] this->points[i];
        }
//        for (int i = 0; i < this->pointsX.size(); i++) {
//            delete[] this->pointsX[i];
//        }
//        for (int i = 0; i < this->pointsY.size(); i++) {
//            delete[] this->pointsY[i];
//        }
//        delete[] this->points;
    }
};

class Data {
public :
    void randomNumberToFile(string filePath, int num, int range) {
        ofstream outfile(filePath);
        srand((unsigned int) time(0));
        for (int i = 0; i < num; i++) {
            outfile << rand() % range << " " << rand() % range << endl;
        }
        outfile.close();
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;
//    Data d;
//    d.randomNumberToFile("D://points.txt", 10, 20);
    Solution s;
    s.readFromFile("D://points.txt");
    s.sortByX();
    s.printPoints();
    s.solveByCommon();
    s.printResult();
    return 0;
}