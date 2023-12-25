#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include<iomanip>
#define cout std::cout
#define cin std::cin
using namespace std;

// 定义航班信息结构体
struct Flight {
    string flightNumber;//班次
    string departureTime;//起飞时间
    string startPoint;//起点
    string endPoint;//终点
    double flightDuration;//飞行时间
    int maxCapacity;//额定载量
    int bookedPassengers;//已定票人数
    bool hasTakenOff;//是否起飞
};

// 航班管理类
class FlightManager {
private:
    vector<Flight> flights; // 存储所有航班信息

public:
    bool flightsEmpty() {
        return flights.empty();
    }
    
    void addFlight() {
        Flight flight;

        cout << "请输入航班号: ";
        cin >> flight.flightNumber;

        cout << "请输入起飞时间 (例如 08:00): ";
        cin >> flight.departureTime;

        cout << "请输入起点站: ";
        cin >> flight.startPoint;

        cout << "请输入终点站: ";
        cin >> flight.endPoint;

        cout << "请输入预计飞行时间（小时）: ";
        cin >> flight.flightDuration;

        cout << "请输入最大载客量: ";
        cin >> flight.maxCapacity;

        flight.bookedPassengers = 0;  // 新航班默认已定票人数为0
        flight.hasTakenOff = false;   // 新航班默认尚未起飞

        // 添加航班信息到列表
        flights.push_back(flight);
        cout << "航班信息已添加.\n";
    }



    void saveFlightsToFileOverwrite(const string& filename) {
        if (flights.empty()) {
            cerr << "没有需要保存的内容！" << endl;
            return;
        }
        ofstream file(filename, ios::out); // 使用ios::out打开文件，覆盖任何现有内容
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return;
        }

        // 写入标题行
        file << "FlightNumber,DepartureTime,StartPoint,EndPoint,FlightDuration,MaxCapacity,BookedPassengers,HasTakenOff\n";

        // 写入数据...
        for (const auto& flight : flights) {
            file << flight.flightNumber << ","
                << flight.departureTime << ","
                << flight.startPoint << ","
                << flight.endPoint << ","
                << flight.flightDuration << ","
                << flight.maxCapacity << ","
                << flight.bookedPassengers << ","
                << (flight.hasTakenOff ? "true" : "false") << "\n";
        }
        file.close();

        cout << "保存成功！以下是目前所有的航班信息：";
        loadFlightsFromFile("D:\\dat\\flights.txt");
        displayAllFlights();
    }

    
    void saveFlightsToFileAppend(const string& filename) {
        ofstream file(filename, ios::app); // 使用ios::app打开文件以追加数据
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return;
        }

        // 写入数据...
        for (const auto& flight : flights) {
            file << flight.flightNumber << ","
                << flight.departureTime << ","
                << flight.startPoint << ","
                << flight.endPoint << ","
                << flight.flightDuration << ","
                << flight.maxCapacity << ","
                << flight.bookedPassengers << ","
                << (flight.hasTakenOff ? "true" : "false") << "\n";
        }
        file.close();
    }

    void displayAllFlights() {
        cout << "\n显示所有航班信息:\n";
        cout <<endl;
        cout << left;  // 左对齐、
        int sum = 0;
        // 设置列宽，并为每列设置标题
        cout << setw(15) << "班次"
            << setw(20) << "起飞时间"
            << setw(15) << "起点站"
            << setw(15) << "终点站"
            << setw(15) << "飞行时间"
            << setw(15) << "额定载量"
            << setw(20) << "已定票人数"
            << setw(10) << "是否起飞" << endl;
        cout << string(120, '-') << endl; 
        cout << endl;
        for (const auto& flight : flights) {
            cout << setw(15) << flight.flightNumber
                << setw(20) << flight.departureTime
                << setw(15) << flight.startPoint
                << setw(15) << flight.endPoint
                << setw(15) << flight.flightDuration
                << setw(15) << flight.maxCapacity
                << setw(20) << flight.bookedPassengers
                << setw(10) << (flight.hasTakenOff ? "Yes" : "No") << endl;
            sum++;
        }
        cout << endl;
        cout << string(51, '-') << "共查询到 " << sum << " 条记录" << string(52, '-') << endl;;
    }



    void loadFlightsFromFile(const string& filename) {
        flights.clear();
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return;
        }

        string line;
        // 读取并忽略标题行
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            Flight flight;
            int field = 0;

            while (getline(ss, value, ',')) {
                switch (field) {
                case 0: flight.flightNumber = value; break;
                case 1: flight.departureTime = value; break;
                case 2: flight.startPoint = value; break;
                case 3: flight.endPoint = value; break;
                case 4: flight.flightDuration = stod(value); break;
                case 5: flight.maxCapacity = stoi(value); break;
                case 6: flight.bookedPassengers = stoi(value); break;
                case 7: flight.hasTakenOff = (value == "1" || value == "true"); break;
                }
                field++;
            }
            flights.push_back(flight);
        }
        file.close();
    }


    void browseFlights() {
        for (const auto& flight : flights) {
            cout << "找到您查询的航班: " << flight.flightNumber << " - "
                << "从 " << flight.startPoint << " 出发到 " << flight.endPoint
                << " 出发时间: " << flight.departureTime << "\n";
        }
    }

    void searchByFlightNumber(string flightNumber) {
        for (const auto& flight : flights) {
            if (flight.flightNumber == flightNumber) {
                cout << "找到您查询的航班: " << flightNumber << " - "
                    << "从 " << flight.startPoint << " 出发到: " << flight.endPoint
                    << " 出发时间: " << flight.departureTime << "\n";
                return;
            }
        }
        cout << "未找到您要查询的航班信息.\n";
    }

    void searchByEndPoint(const string& endPoint) {
        for (const auto& flight : flights) {
            if (flight.endPoint == endPoint) {
                cout << "航班飞往 " << endPoint << ": " << flight.flightNumber << " - "
                    << "出发时间: " << flight.departureTime << "\n";
            }
        }
    }

    void bookTicket(string flightNumber) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff && flight.bookedPassengers < flight.maxCapacity) {
                flight.bookedPassengers++;
                saveFlightsToFileOverwrite("D:\\dat\\flights.txt");
                cout << "本次航班购票成功 " << flightNumber << "\n";
                return;
            }
        }
        cout << "无法订购本次航班，班次不存在或已达额定载客量！ " << flightNumber << "\n";
    }

    void cancelTicket(string flightNumber) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff && flight.bookedPassengers > 0) {
                flight.bookedPassengers--;
                saveFlightsToFileOverwrite("D:\\dat\\flights.txt");
                cout << "订票已取消 " << flightNumber << "\n";
                return;
            }
        }
        cout << "无法取消本次订票，班次号不存在或航班已发出！ " << flightNumber << "\n";
    }

    void modifyFlight(string flightNumber) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber) {
                cout << "找到航班 " << flightNumber << ". 请输入新的信息 (直接按回车保留原值):\n";

                string input;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清除之前留下的换行符

                cout << "新的起飞时间 (当前为 " << flight.departureTime << "): ";
                getline(cin, input);
                if (!input.empty()) flight.departureTime = input;

                cout << "新的起点站 (当前为 " << flight.startPoint << "): ";
                getline(cin, input);
                if (!input.empty()) flight.startPoint = input;

                cout << "新的终点站 (当前为 " << flight.endPoint << "): ";
                getline(cin, input);
                if (!input.empty()) flight.endPoint = input;

                cout << "新的飞行时间（小时） (当前为 " << flight.flightDuration << "): ";
                getline(cin, input);
                if (!input.empty()) {
                    stringstream ss(input);
                    double newDuration;
                    ss >> newDuration;
                    if (!ss.fail()) {
                        flight.flightDuration = newDuration;
                    }
                }

                cout << "新的最大载客量 (当前为 " << flight.maxCapacity << "): ";
                getline(cin, input);
                if (!input.empty()) {
                    stringstream ss(input);
                    int newMaxCapacity;
                    ss >> newMaxCapacity;
                    if (!ss.fail()) {
                        flight.maxCapacity = newMaxCapacity;
                    }
                }

                cout << "是否已起飞 (y/n) (当前为 " << (flight.hasTakenOff ? "Yes" : "No") << "): ";
                getline(cin, input);
                if (!input.empty()) {
                    flight.hasTakenOff = (input == "y" || input == "Y");
                }

                cout << "航班信息已更新.\n";
                return;
            }
        }
        cout << "未找到航班号为 " << flightNumber << " 的航班.\n";
    }



    void deleteFlight(string flightNumber) {
        for (auto it = flights.begin(); it != flights.end(); ++it) {
            if (it->flightNumber == flightNumber) {
                flights.erase(it);
                cout << "航班 " << flightNumber << " 已被删除.\n";
                cout << "请使用保存选项保存更改！" << endl;
                return;
            }
        }
        cout << "未找到航班号为 " << flightNumber << " 的航班.\n";
    }

    int countLinesInFile(const string& filename) {
        ifstream file(filename);
        int lineCount = 0;
        string line;

        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return -1;
        }

        while (std::getline(file, line)) {
            ++lineCount;
        }

        file.close();
        return lineCount-1;
    }
};

// 用户界面函数
void userInterface(FlightManager& manager) {
    int choice;
    cout << string(50, '-') << "欢迎使用机票管理系统" << string(50, '-') << endl;
    cout<<endl;
    // 检查flights是否为空
    if (manager.flightsEmpty()) {
        cout << "当前没有加载的航班信息。是否从文件加载? (y/n): ";
        char loadChoice;
        cin >> loadChoice;
        if (loadChoice == 'y' || loadChoice == 'Y') {
            string filename;
            cout << "请输入文件地址: ";
            cin >> filename;
            manager.loadFlightsFromFile(filename);
            cout << "成功从 "<<filename<<" 加载！" << endl;
            cout << "共读取到 " << manager.countLinesInFile(filename) << " 条记录！" << endl;
        }
    }

    do {
        cout << "\n*** 机票管理系统 ***\n";
        cout << "1. 添加航班信息\n";
        cout << "2. 保存航班信息\n";
        cout << "3. 加载航班信息\n";
        cout << "4. 浏览班次信息\n";
        cout << "5. 使用班次号查询路线\n";
        cout << "6. 使用终点站查询路线\n";
        cout << "7. 订票\n";
        cout << "8. 退票\n";
        cout << "9. 修改航班信息\n";
        cout << "10. 删除航班\n";
        cout << "0.退出系统\n";
        cout << "请输入数字选择: ";
        cin >> choice;

        string flightNumber;
        string endPoint;
        switch (choice) {
        case 1: {
            manager.addFlight();
            //cout << "添加成功！";
            //manager.saveFlightsToFileAppend("D:\\dat\\flights.txt");
            break;
        }
        case 2:
            manager.saveFlightsToFileOverwrite("D:\\dat\\flights.txt");
            break;
        case 3:
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            cout << "加载成功！以下是目前所有的航班信息：";
            manager.displayAllFlights();
            break;
        case 4:
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            //manager.browseFlights();
            manager.displayAllFlights();
            break;
        case 5:
            cout << "输入班次号以搜索: ";
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            cin >> flightNumber;
            manager.searchByFlightNumber(flightNumber);
            break;
        case 6:
            cout << "输入终点站以搜索: ";
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            cin >> endPoint;
            manager.searchByEndPoint(endPoint);
            break;
        case 7:
            cout << "输入航班号以订票: ";
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            cin >> flightNumber;
            manager.bookTicket(flightNumber);
            break;
        case 8:
            cout << "输入航班号来取消订票: ";
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");
            cin >> flightNumber;
            manager.cancelTicket(flightNumber);
            break;
        case 9:
            cout << "请输入要修改的航班号: ";
            cin >> flightNumber;
            manager.modifyFlight(flightNumber);
            break;
        case 10:
            cout << "请输入要删除的航班号: ";
            cin >> flightNumber;
            manager.deleteFlight(flightNumber);
            break;
        }
        if (choice != 0) {
            cout << "\n按下回车以返回主菜单";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get(); 
        }
    } while (choice != 0);
}

int main() {
    FlightManager manager;
    userInterface(manager);
    return 0;
}
