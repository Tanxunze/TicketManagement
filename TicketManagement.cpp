#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include<iomanip>
#define cout std::cout
#define cin std::cin
using namespace std;

struct SeatPricing {
    string classType;  // 如 "经济舱", "商务舱", "头等舱"
    double price;      // 对应的票价
};
// 定义航班信息结构体
struct Flight {
    string flightNumber;
    string departureTime;
    string startPoint;
    string endPoint;
    double flightDuration;
    int maxCapacity;
    int bookedPassengers;
    bool hasTakenOff;
    string airline;
    vector<SeatPricing> seatPricing; 
};


// 航班管理类
class FlightManager {
private:
    vector<Flight> flights; // 存储所有航班信息

public:
    bool isExist = true;
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

        // 新增 - 输入航空公司信息
        cout << "请输入航空公司: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除输入缓冲区
        getline(cin, flight.airline);

        flight.bookedPassengers = 0;  // 新航班默认已定票人数为0
        flight.hasTakenOff = false;   // 新航班默认尚未起飞

        // 输入不同舱位的票价信息
        int pricingCount;
        cout << "请输入舱位等级数量: ";
        cin >> pricingCount;
        for (int i = 0; i < pricingCount; ++i) {
            SeatPricing pricing;
            cout << "请输入舱位类型（如经济舱、商务舱）: ";
            cin >> ws;  // 忽略前面的空白字符
            getline(cin, pricing.classType);
            cout << "请输入" << pricing.classType << "的票价: ";
            cin >> pricing.price;
            flight.seatPricing.push_back(pricing);
        }

        // 添加航班信息到列表
        flights.push_back(flight);
        cout << "航班信息已添加。\n";
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

        // 写入标题行，添加舱位等级和票价的标题
        file << "班次,起飞时间,起点站,终点站,飞行时间,额定载量,已订票人数,是否起飞,航空公司,舱位等级和票价\n";

        // 写入数据
        for (const auto& flight : flights) {
            file << flight.flightNumber << ","
                << flight.departureTime << ","
                << flight.startPoint << ","
                << flight.endPoint << ","
                << flight.flightDuration << ","
                << flight.maxCapacity << ","
                << flight.bookedPassengers << ","
                << (flight.hasTakenOff ? "true" : "false") << ","
                << flight.airline;

            // 对于每个航班，写入舱位等级和票价信息
            for (const auto& pricing : flight.seatPricing) {
                file << " " << pricing.classType << ":" << pricing.price;
            }
            file << "\n";  // 每条航班记录结束后换行
        }
        file.close();

        cout << "保存成功！以下是目前所有的航班信息：\n";
        loadFlightsFromFile(filename);
        displayAllFlights();
    }


    
    /*void saveFlightsToFileAppend(const string& filename) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return;
        }
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
    }*/

    void displayAllFlights() {
        cout << "\n显示所有航班信息:\n";
        cout << left;  // 左对齐
        int sum = 0;
        // 设置列宽，并为每列设置标题
        cout << setw(10) << "班次"
            << setw(15) << "起飞时间"
            << setw(10) << "起点站"
            << setw(10) << "终点站"
            << setw(10) << "飞行时间"
            << setw(10) << "额定载量"
            << setw(15) << "已定票人数"
            << setw(10) << "是否起飞"
            << setw(20) << "航空公司"
            << setw(30)<<"舱位等级和票价" << endl;
        cout << string(150, '-') << endl;
        cout << endl;
        for (const auto& flight : flights) {
            cout << setw(10) << flight.flightNumber
                << setw(15) << flight.departureTime
                << setw(10) << flight.startPoint
                << setw(10) << flight.endPoint
                << setw(10) << flight.flightDuration
                << setw(10) << flight.maxCapacity
                << setw(15) << flight.bookedPassengers
                << setw(10) << (flight.hasTakenOff ? "是" : "否")
                << setw(20) << flight.airline;

            for (const auto& pricing : flight.seatPricing) {
                cout << pricing.classType << ": "  << pricing.price << " ";
            }
            cout << endl;  // 完成该航班信息的显示，换行
            sum++;
        }

        cout << endl;
        cout << string(60, '-') << "共查询到 " << sum << " 条记录" << string(73, '-') << endl;
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
                if (field < 8) {  // 处理前8个基本字段
                    switch (field) {
                    case 0: flight.flightNumber = value; break;
                    case 1: flight.departureTime = value; break;
                    case 2: flight.startPoint = value; break;
                    case 3: flight.endPoint = value; break;
                    case 4: flight.flightDuration = stod(value); break;
                    case 5: flight.maxCapacity = stoi(value); break;
                    case 6: flight.bookedPassengers = stoi(value); break;
                    case 7: flight.hasTakenOff = (value == "true"); break;
                    }
                }
                else if (field == 8) {  // 处理航空公司
                    flight.airline = value;
                }
                else {  // 处理舱位等级和票价
                    stringstream ssp(value);
                    string pricingInfo;
                    while (getline(ssp, pricingInfo, ';')) {
                        stringstream sspr(pricingInfo);
                        string classType, price;
                        getline(sspr, classType, ':');
                        getline(sspr, price);
                        SeatPricing pricing{ classType, stod(price) };
                        flight.seatPricing.push_back(pricing);
                    }
                }
                field++;
            }
            flights.push_back(flight);
        }
        file.close();
        cout << "加载成功！共加载 " << flights.size() << " 条航班信息。\n";
    }



    void browseFlights(string flightNumber) {
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
        cout << "请输入要订票的航班号: ";
        cin >> flightNumber;

        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff) {
                // 显示可用舱位和票价
                cout << "可选舱位和票价: \n";
                for (const auto& pricing : flight.seatPricing) {
                    cout << pricing.classType << ": " << pricing.price << "元\n";
                }

                // 让用户选择舱位
                string chosenClass;
                cout << "请输入选择的舱位类型: ";
                cin >> chosenClass;

                // 验证舱位是否存在
                bool classFound = false;
                for (const auto& pricing : flight.seatPricing) {
                    if (pricing.classType == chosenClass) {
                        classFound = true;
                        // 检查是否还有座位
                        if (flight.bookedPassengers < flight.maxCapacity) {
                            flight.bookedPassengers++;  // 增加已订票人数
                            cout << "已成功订购" << chosenClass << "，票价为: " << pricing.price << "元\n";
                        }
                        else {
                            cout << "抱歉，该航班已满员。\n";
                        }
                        break;
                    }
                }

                if (!classFound) {
                    cout << "未找到所选舱位类型，请重新选择。\n";
                }
                return;
            }
        }
        cout << "未找到航班号为 " << flightNumber << " 的航班或航班已起飞。\n";
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
                cout << "找到航班 " << flightNumber << ". 请输入新的信息 (直接按回车可保留原值):\n";

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
                // 新增 - 修改舱位票价信息
                cout << "当前舱位票价信息:\n";
                for (const auto& pricing : flight.seatPricing) {
                    cout << pricing.classType << ": " << pricing.price << "\n";
                }
                cout << "是否修改舱位票价信息? (y/n): ";
                getline(cin, input);
                if (input == "y" || input == "Y") {
                    flight.seatPricing.clear();  // 清除旧的票价信息
                    cout << "请输入新的舱位等级数量: ";
                    int seatTypes;
                    cin >> seatTypes;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    for (int i = 0; i < seatTypes; i++) {
                        SeatPricing pricing;
                        cout << "请输入舱位类型（如经济舱、商务舱）: ";
                        getline(cin, pricing.classType);
                        cout << "请输入" << pricing.classType << "的票价: ";
                        getline(cin, input);
                        stringstream ss(input);
                        ss >> pricing.price;
                        flight.seatPricing.push_back(pricing);
                    }
                }

                cout << "航班信息已更新.\n";
                return;
            }
        }
        cout << "未找到航班号为 " << flightNumber << " 的航班。\n";
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
            if (manager.isExist) {
                cout << "成功从 " << filename << " 加载！" << endl;
                cout << "共读取到 " << manager.countLinesInFile(filename) << " 条记录！" << endl;
                manager.isExist = true;
            }
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
            //cout << "输入航班号以订票: ";
            //manager.loadFlightsFromFile("D:\\dat\\flights.txt");
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
