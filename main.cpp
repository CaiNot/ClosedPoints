#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>
#include <windows.h>
#include <algorithm>

using namespace std;

int square(int x) {
    return x * x;
}

int distance(int *point_1, int *point_2) {
    return square(point_1[0] - point_2[0]) + square(point_1[1] - point_2[1]);
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
    vector<vector<int *>> groups;
    vector<int *> points;
    vector<int *> pointsX; // 以X为基准 升序
    vector<int *> pointsY; // 以Y为基准 升序
    vector<int *> pointsT;
    int minDis; // 保存最短距离
    int point_1[2], point_2[2]; // 保存最近点对

    vector<int> result_XY;

public:
    Solution() : pointsNum(0), points(0) {
        this->minDis = INT_MAX;
        this->point_1[0] = -1;
        this->point_1[1] = -1;
        this->point_2[0] = -1;
        this->point_2[1] = -1;
    }

//    bool readFromFile(string filePath) {
//        int x, y;
//        string line;
//        ifstream infile(filePath);
//        stringstream ss;
//
//        if (!infile.is_open()) {
//            cout << "File Open Error" << endl;
//            return false;
//        }
//
//        int pos = 0;
//        int *posTemp = 0;
//        while (!infile.eof()) {
//            getline(infile, line);
//            ss.clear();
//            ss.str(line);
//            ss >> x >> y;
//            if (ss.fail()) {
//                continue;
//            }
//            posTemp = new int[2];
//            posTemp[0] = x;
//            posTemp[1] = y;
//            if (y == 25918) {
//                cout << "x: " << x << " y: " << y << endl;
//
//            }
//            this->points.push_back(posTemp);
//
//            pos++;
//
//            //            cout << "x: " << x << " y: " << y << endl;
//        }
//        this->pointsNum = pos;
//        cout << "the num of points is " << pos << endl;
//
//        infile.close();
//        return true;
//    }

    bool readFromFile(string filePath) {

        int x, y;
        string line;
        ifstream infile(filePath);
        stringstream ss;

        if (!infile.is_open()) {
            cout << "File Open Error" << endl;
            return false;
        }

        int pos = 0;
        int *posTemp = 0;

        int groupNum = 0;
        getline(infile, line);
        ss.clear();
        ss.str(line);
        ss >> groupNum;
        if (ss.fail()) {
            cout << "File Read Error" << endl;
            return false;
        }
        int oneTestNum = 0;
        for (int i = 0; i < groupNum; i++) {
            getline(infile, line);
            ss.clear();
            ss.str(line);
            ss >> oneTestNum;
            if (ss.fail()) {
                cout << "File Read Error" << endl;
                return false;
            }
            pos = 0;
            for (int j = 0; j < oneTestNum; j++) {
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
            }
            this->groups.push_back(this->points);
            this->points.clear();
        }


        infile.close();
        return true;
    }

    void solveByCommon() {
        for (int i = 0; i < this->groups.size(); i++) {
            this->points = this->groups[i];
            this->pointsNum = this->points.size();
            int disTemp = 0;
            this->minDis = INT_MAX;
            for (int i = 0; i < this->pointsNum; i++) {
                for (int j = i + 1; j < this->pointsNum; j++) {
                    disTemp = distance(this->points[i], this->points[j]);
                    if (disTemp < this->minDis) {
                        this->minDis = disTemp;
                        this->point_1[0] = this->points[i][0];
                        this->point_1[1] = this->points[i][1];
                        this->point_2[0] = this->points[j][0];
                        this->point_2[1] = this->points[j][1];
                    }
                }
            }
            this->printResult(i + 1);
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

    void sortByY(vector<int *> &my_points, int start, int end) {
        this->pointsT.assign(my_points.begin() + start, my_points.begin() + end);
        sort(this->pointsT.begin(), this->pointsT.end(), cmpY);
        //        this->pointsY.assign(this->points.begin(), this->points.end());
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
        ofstream saveData("out.dat");
        for (int i = 0; i < this->groups.size(); i++) {
            this->points = this->groups[i];
            this->pointsNum = this->points.size();
            this->minDis = INT_MAX;

            this->sortByX();
            //        this->sortByY();
            minDisArea(0, this->pointsNum - 1);
            this->printResult(i + 1);
            for (int j = 0; j < this->result_XY.size(); j++) {
                saveData << this->result_XY[j] << "\t";
            }
            saveData << endl;
        }
    }


    int minDisArea(int start, int end) {
        if (start < end) {
            int mid = (start + end) / 2;

            for (; mid > start + 1 && this->pointsX[mid][0] == this->pointsX[mid - 1][0];) {
                mid--;
            }
            int minDisL = minDisArea(start, mid);
            int minDisR = minDisArea(mid + 1, end);

            this->minDis = min(minDisL, minDisR);
            int startMid = mid, endMid = mid; // 问题好像出在这里
            for (; startMid > start && (this->pointsX[startMid][0] + this->minDis) >= this->pointsX[mid][0];) {
                startMid--;
            }
            for (; endMid < end && (this->pointsX[endMid][0] - this->minDis) <= this->pointsX[mid][0];) {
                endMid++;
            }

            int minDisM = minDisMid(startMid, endMid);
            this->minDis = min(this->minDis, minDisM);;
        } else {
            int disTemp = 0;
            for (int i = start; i <= end; i++) {
                for (int j = i + 1; j <= end; j++) {

                    disTemp = distance(this->pointsX[i], this->pointsX[j]);
                    if (this->minDis >= disTemp) {
                        if (this->minDis != disTemp) {
                            this->result_XY.clear();
                        } else if (this->isSame(this->pointsX[i][0], this->pointsX[i][1], pointsX[j][0], pointsX[j][1]))
                            continue;
                        this->minDis = disTemp;

                        this->point_1[0] = this->pointsX[i][0];
                        this->point_1[1] = this->pointsX[i][1];
                        this->point_2[0] = this->pointsX[j][0];
                        this->point_2[1] = this->pointsX[j][1];

                        this->result_XY.push_back(this->pointsX[i][0]);
                        this->result_XY.push_back(this->pointsX[i][1]);
                        this->result_XY.push_back(this->pointsX[j][0]);
                        this->result_XY.push_back(this->pointsX[j][1]);
                    }
                }
            }
        }
        return this->minDis;
    }

    int minDisMid(int start, int end) {
        int disTemp = 0, len = end - start;
        this->sortByY(this->pointsX, start, end + 1);
        for (int i = 0; i <= len; i++) {
            for (int j = i + 1; j <= len && j <= 7 + i; j++) {

                if (abs(this->pointsT[i][1] - this->pointsT[j][1]) > this->minDis) {
                    break;
                }
                disTemp = distance(this->pointsT[i], this->pointsT[j]);
                if (this->minDis >= disTemp) {
                    if (this->minDis != disTemp) {
                        this->result_XY.clear();
                    } else if (this->isSame(this->pointsT[i][0], this->pointsT[i][1], pointsT[j][0], pointsT[j][1]))
                        continue;
                    this->minDis = disTemp;
                    this->point_1[0] = this->pointsT[i][0];
                    this->point_1[1] = this->pointsT[i][1];
                    this->point_2[0] = this->pointsT[j][0];
                    this->point_2[1] = this->pointsT[j][1];

                    this->result_XY.push_back(this->pointsT[i][0]);
                    this->result_XY.push_back(this->pointsT[i][1]);
                    this->result_XY.push_back(this->pointsT[j][0]);
                    this->result_XY.push_back(this->pointsT[j][1]);
                }
            }
        }
        return this->minDis;
    }

    bool isSame(int x_1, int y_1, int x_2, int y_2) {
        return this->point_1[0] == x_1 && this->point_1[1] == y_1
               && this->point_2[0] == x_2 && this->point_2[1] == y_2;
    }

    void printResult(int i = 0) {
        cout << "the closest pair of points in group " << i << " is:" << endl;
        cout << "Point_1:\tx: " << this->point_1[0] << " y: " << this->point_1[1] << endl;
        cout << "Point_2:\tx: " << this->point_2[0] << " y: " << this->point_2[1] << endl;
        cout << "the distance's square between two points is " << this->minDis << endl << endl;
    }

    void printPoints() {
        cout << "All Points: " << endl;
        for (int i = 0; i < this->pointsNum; i++) {
            cout << "x: " << this->points[i][0] << " y: " << this->points[i][1] << endl;
        }
    }

    ~Solution() {
        for (int j = 0; j < this->groups.size(); j++) {
            this->points = this->groups[j];
            for (int i = 0; i < this->points.size(); i++) {
                delete[] this->points[i];
            }
        }
    }
};

class Data {
public:
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
    //    std::cout << "Hello, World!" << std::endl;
//    Data d;
//    d.randomNumberToFile("D://points.txt", 5000, 600000);

    Solution s;
    s.readFromFile("in.dat");


    DWORD start, end;
    DWORD totalTime;

//    start = GetTickCount();
    cout << "分治法: " << endl;
    s.solveByDivider();
//    end = GetTickCount();
//    totalTime = end - start;
//    cout << "分治法的时间: " << totalTime << endl;
//    s.printResult();

//    start = GetTickCount();
    cout << "搜索法: " << endl;
    s.solveByCommon();
//    end = GetTickCount();
//    totalTime = end - start;
//    cout << "搜索法的时间: " << totalTime << endl;

//    s.printResult();
    return 0;
}