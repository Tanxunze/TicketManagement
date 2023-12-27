#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include<iomanip>
#define cout std::cout
#define cin std::cin
using namespace std;

struct Booking {
    string flightNumber;
    string classType; 
    double price;
};
struct SeatPricing {
    string classType; 
    double price;      
};

struct User {
    string username;
    string password;
    string role;  // "admin" 或 "user"
    vector<Booking> bookings; 
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
public:
    vector<Flight> flights; 
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
                << flight.airline<<",";

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
            << setw(20)<<"舱位等级和票价" << endl;
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
                cout <<" " << pricing.classType << ": " << pricing.price << " ";
            }
            cout << endl;  
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

            // 读取前8个基本字段
            while (field < 8 && getline(ss, value, ',')) {
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
                field++;
            }

            // 读取航空公司字段
            getline(ss, flight.airline, ',');

            // 跳过舱位信息前的逗号
            //getline(ss, value, ',');  // 这里读取的是舱位信息前的逗号之后的部分

            // 读取舱位信息
            string seatInfo;
            getline(ss, seatInfo);  // 获取整个舱位信息字符串
            stringstream ssp(seatInfo);
            string pricingInfo;
            while (getline(ssp, pricingInfo, ' ')) {  // 使用空格分隔不同舱位信息
                if (pricingInfo.empty()) continue; // 忽略连续的空格
                stringstream sspr(pricingInfo);
                string classType, price;
                getline(sspr, classType, ':');
                getline(sspr, price);
                if (!classType.empty() && !price.empty()) {
                    SeatPricing pricing{ classType, stod(price) };
                    flight.seatPricing.push_back(pricing);
                }
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

    void bookTicket() {
        string flightNumber;
        cout << "请输入要订票的航班号: ";
        cin >> flightNumber;

        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff) {
                // 显示可用舱位和票价
                cout << "可选舱位和票价: \n";
                for (const auto& pricing : flight.seatPricing) {
                    cout << " " << pricing.classType << ": " << pricing.price << " ";
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

class UserAction {
public:
    void registerUser() {
        string username, password;
        cout << "创建新用户\n";
        cout << "输入用户名: ";
        cin >> username;
        cout << "输入密码: ";
        cin >> password;

        // 检查用户名是否已存在
        ifstream inFile("D:\\dat\\users.txt");
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string existingUser, _, __;
            getline(ss, existingUser, ',');
            if (username == existingUser) {
                cout << "用户名已存在，请选择其他用户名。\n";
                return;
            }
        }
        inFile.close();

        // 将新用户信息写入文件
        ofstream outFile("D:\\dat\\users.txt", ios::app);
        outFile << username << "," << password << ",user\n";  // 默认为普通用户
        outFile.close();

        cout << "注册成功！请使用新用户名和密码登录。\n";
    }



    User login() {
        string username, password;
        cout << "用户登录\n";
        cout << "输入用户名: ";
        cin >> username;
        cout << "输入密码: ";
        cin >> password;

        // 从文件中读取用户信息并验证
        ifstream inFile("D:\\dat\\users.txt");
        string line;
        bool userFound = false;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string existingUser, pass, role;
            getline(ss, existingUser, ',');
            getline(ss, pass, ',');
            getline(ss, role, '|');  // 假设角色和订票信息之间用'|'分隔

            if (username == existingUser) {
                userFound = true;
                if (password == pass) {
                    cout << "登录成功！欢迎 " << role << " " << username << "\n";
                    inFile.close();
                    return User{ existingUser, pass, role };  // 返回登录的用户对象
                }
                else {
                    cout << "密码错误！\n";
                    inFile.close();
                    return User{ "", "", "" };  // 登录失败，返回空用户对象
                }
            }
        }
        inFile.close();

        if (!userFound) {
            cout << "用户名不存在！\n";
        }
        return User{ "", "", "" };  // 登录失败，返回空用户对象
    }



    //订票与退票函数
    void bookTicketForUser(User& user, vector<Flight>& flights) {
        string flightNumber;
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
                            
                            // 添加到用户的订票信息中
                            user.bookings.push_back({ flight.flightNumber, chosenClass, pricing.price });
                            cout << "已成功订购" << chosenClass << "，票价为: " << pricing.price << "元\n";
                            // 更新用户文件（假设 updateUserFile 函数已定义）
                            updateUserFile(user);
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
    void cancelTicketForUser(const string& username, const string& flightNumber, const string& classType) {
        FlightManager manager;
        // 首先读取所有用户信息并寻找目标用户和其订票记录
        ifstream userFile("D:\\dat\\users.txt");
        vector<User> allUsers;
        string line;
        bool userFound = false;
        bool ticketFound = false;

        while (getline(userFile, line)) {
            stringstream ss(line);
            User user;
            string bookingsInfo;
            getline(ss, user.username, ',');
            getline(ss, user.password, ',');
            getline(ss, user.role, '|');

            // 解析用户的订票信息
            while (getline(ss, bookingsInfo, '|')) {
                stringstream bookingSS(bookingsInfo);
                Booking booking;
                string fNumber, cType, price;

                getline(bookingSS, fNumber, ':');
                getline(bookingSS, cType, ':');
                getline(bookingSS, price);

                if (!fNumber.empty() && !cType.empty() && !price.empty()) {
                    booking.flightNumber = fNumber;
                    booking.classType = cType;
                    booking.price = stod(price);
                    user.bookings.push_back(booking);

                    // 检查是否是要取消的订票
                    if (user.username == username && booking.flightNumber == flightNumber && booking.classType == classType) {
                        ticketFound = true;
                    }
                }
            }

            allUsers.push_back(user);
            if (user.username == username) {
                userFound = true;
            }
        }
        userFile.close();

        // 如果找到用户和订票记录，则进行取消操作
        if (userFound && ticketFound) {
            for (auto& user : allUsers) {
                if (user.username == username) {
                    // 移除找到的订票记录
                    user.bookings.erase(std::remove_if(user.bookings.begin(), user.bookings.end(),
                        [&](const Booking& b) { return b.flightNumber == flightNumber && b.classType == classType; }),
                        user.bookings.end());
                    break;
                }
            }

            // 更新users.txt文件
            ofstream outFile("D:\\dat\\users.txt");
            for (const auto& u : allUsers) {
                outFile << u.username << "," << u.password << "," << u.role;
                for (const auto& booking : u.bookings) {
                    outFile << "|" << booking.flightNumber << ":" << booking.classType << ":" << booking.price;
                }
                outFile << endl;
            }
            outFile.close();

            // 同时更新flights.txt文件
            manager.loadFlightsFromFile("D:\\dat\\flights.txt");  
            for (auto& flight : manager.flights) {
                if (flight.flightNumber == flightNumber && !flight.hasTakenOff && flight.bookedPassengers > 0) {
                    flight.bookedPassengers--;
                }
            }
            manager.saveFlightsToFileOverwrite("D:\\dat\\flights.txt");  

            cout << "订票已取消 " << flightNumber << "，舱位 " << classType << "\n";
        }
        else {
            cout << "未找到用户或订票记录，无法取消订票。\n";
        }
    }


    void updateUserFile(const User& user) {
        vector<User> allUsers;
        ifstream inFile("D:\\dat\\users.txt");
        string line;

        // 首先，从文件中读取所有用户的信息
        while (getline(inFile, line)) {
            stringstream ss(line);
            string username, password, role, bookingsInfo;
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, role, ',');

            User u{ username, password, role, {} };

            // 解析用户的订票信息
            while (getline(ss, bookingsInfo, '|')) {
                stringstream bookingSS(bookingsInfo);
                string flightNumber, classType, price;

                getline(bookingSS, flightNumber, ':');
                getline(bookingSS, classType, ':');
                getline(bookingSS, price);

                if (!flightNumber.empty() && !classType.empty() && !price.empty()) {
                    Booking booking{ flightNumber, classType, stod(price) };
                    u.bookings.push_back(booking);
                }
            }

            allUsers.push_back(u);
        }
        inFile.close();

        // 然后，更新指定用户的信息
        for (auto& u : allUsers) {
            if (u.username == user.username) {
                u = user;
                break;
            }
        }

        // 最后，将所有用户的信息写回文件
        ofstream outFile("D:\\dat\\users.txt");
        for (const auto& u : allUsers) {
            outFile << u.username << "," << u.password << "," << u.role;
            for (const auto& booking : u.bookings) {
                outFile << "|" << booking.flightNumber << ":" << booking.classType << ":" << booking.price;
            }
            outFile << std::endl;
        }
        outFile.close();
    }


    void adminPage(User& user) {
        FlightManager manager;
        int choice;
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
                manager.loadFlightsFromFile("D:\\dat\\flights.txt");
                manager.bookTicket();
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

    void showUserBookings(string& username, const string& filename) {
        ifstream inFile(filename);
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            User user;
            string bookingsInfo;

            getline(ss, user.username, ',');
            //cout << user.username << endl;
            getline(ss, user.password, ',');
            //cout << user.password << endl;
            getline(ss, user.role, '|');
            cout << user.role << endl;
            //cout << "debug" << user.username<<endl;
            // 如果找到了用户，则读取和展示订票信息
            if (user.username == username) {
                while (getline(ss, bookingsInfo, '|')) {
                    //cout << "in looped" << endl;
                    stringstream bookingSS(bookingsInfo);
                    Booking booking;
                    string flightNumber, classType, price;

                    getline(bookingSS, flightNumber, ':');
                    getline(bookingSS, classType, ':');
                    getline(bookingSS, price);
                    //cout << "debug:" << flightNumber << " " << classType << " " << price << endl;
                    if (!flightNumber.empty() && !classType.empty() && !price.empty()) {
                        booking.flightNumber = flightNumber;
                        booking.classType = classType;
                        booking.price = stod(price);
                        user.bookings.push_back(booking);
                    }
                }

                cout << "用户 " << username << " 的订票信息:\n";
                if (user.bookings.empty()) {
                    cout << "您当前没有任何订票记录。\n";
                }
                else {
                    for (const auto& booking : user.bookings) {
                        cout << "航班号: " << booking.flightNumber
                            << ", 舱位类型: " << booking.classType
                            << ", 价格: " << booking.price << "元\n";
                    }
                }
                return;
            }
        }
        cout << "未找到用户 " << username << " 的订票信息。\n";
    }

    void saveTickets(string& username, const string& filename) {
        FlightManager manager;
        if (manager.flights.empty()) {
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
        for (const auto& flight : manager.flights) {
            file << flight.flightNumber << ","
                << flight.departureTime << ","
                << flight.startPoint << ","
                << flight.endPoint << ","
                << flight.flightDuration << ","
                << flight.maxCapacity << ","
                << flight.bookedPassengers << ","
                << (flight.hasTakenOff ? "true" : "false") << ","
                << flight.airline << ",";

            // 对于每个航班，写入舱位等级和票价信息
            for (const auto& pricing : flight.seatPricing) {
                file << " " << pricing.classType << ":" << pricing.price;
            }
            file << "\n";  // 每条航班记录结束后换行
        }
        file.close();

        cout << "保存成功！以下是目前订票信息：\n";
        showUserBookings(username, "D:\\dat\\users.txt");
        
    }

    void userPage(User& user) {
        FlightManager manager;
        UserAction useraction;
        Booking booking;
        int choice;
        do {
            cout << "\n*** 用户售票系统 ***\n";
            cout << "1. 浏览班次信息\n";
            cout << "2. 使用班次号查询路线\n";
            cout << "3. 使用终点站查询路线\n";
            cout << "4. 订票\n";
            cout << "5. 退票\n";
            cout << "6.保存订票信息\n";
            cout << "7.查看订票信息\n";
            cout << "0.退出系统\n";
            cout << "请输入数字选择: ";
            cin >> choice;

            string flightNumber;
            string endPoint;
            string classType;
            switch (choice) {
            case 1:
                manager.loadFlightsFromFile("D:\\dat\\flights.txt");
                //manager.browseFlights();
                manager.displayAllFlights();
                break;
            case 2:
                cout << "输入班次号以搜索: ";
                manager.loadFlightsFromFile("D:\\dat\\flights.txt");
                cin >> flightNumber;
                manager.searchByFlightNumber(flightNumber);
                break;
            case 3:
                cout << "输入终点站以搜索: ";
                manager.loadFlightsFromFile("D:\\dat\\flights.txt");
                cin >> endPoint;
                manager.searchByEndPoint(endPoint);
                break;
            case 4:
                manager.loadFlightsFromFile("D:\\dat\\flights.txt");
                useraction.bookTicketForUser(user,manager.flights);
                break;
            case 5:
                cout << "请输入要取消的航班号: ";
                cin >> flightNumber;
                cout << "请输入要取消的舱位类型: ";
                cin >> classType;
                useraction.cancelTicketForUser(user.username,flightNumber,classType);
                break;
            case 6:
                saveTickets(user.username, "D:\\dat\\flights.txt");
            case 7:
                //cout << "debug " << user.username << endl;
                showUserBookings(user.username, "D:\\dat\\users.txt");
            }
            if (choice != 0) {
                cout << "\n按下回车以返回主菜单";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        } while (choice != 0);
    }


};

// 用户界面函数
void userInterface(FlightManager& manager,UserAction& useraction) {
    cout << string(50, '-') << "欢迎使用机票管理系统" << string(50, '-') << endl;
    cout << endl;
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
    while (true) {
        int choice;
        cout << string(50, '-') << "欢迎使用机票管理系统" << string(50, '-') << endl;
        cout << endl;
        cout << "1. 登录\n";
        cout << "2. 注册\n";
        cout << "3. 退出\n";
        cout << "请选择操作 (1-3): ";
        cin >> choice;

        if (choice == 1) {
            User user = useraction.login();  
            if (user.role == "admin") {
                useraction.adminPage(user);  // 跳转到管理员页面
            }
            else if (user.role == "user") {
                useraction.userPage(user);  // 跳转到普通用户页面
            }
        }
        else if (choice == 2) {
            useraction.registerUser();  
            cout << "请登录您的新账户。\n";
        }
        else if (choice == 3) {
            cout << "感谢使用，再见！\n";
            break;  
        }
        else {
            cout << "无效选择，请重新选择。\n";
        }
    }
}

int main() {
    FlightManager manager;
    UserAction useraction;
    userInterface(manager,useraction);
    return 0;
}
