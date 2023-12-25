#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// 定义航班信息结构体
struct Flight {
    int flightNumber;//班次
    string departureTime;//起飞时间
    string startPoint;//起点站
    string endPoint;//终点站
    double flightDuration;
    int maxCapacity;
    int bookedPassengers;
    bool hasTakenOff;
};

// 航班管理类
class FlightManager {
private:
    vector<Flight> flights; // 存储所有航班信息

public:
    void addFlight(const Flight& flight) {
        flights.push_back(flight);
    }

    void saveFlightsToFile(const string& filename) {
        ofstream file(filename, ios::out | ios::binary);
        for (const auto& flight : flights) {
            file.write((char*)&flight, sizeof(Flight));
        }
        file.close();
    }

    void loadFlightsFromFile(const string& filename) {
        Flight flight;
        ifstream file(filename, ios::in | ios::binary);
        while (file.read((char*)&flight, sizeof(Flight))) {
            flights.push_back(flight);
        }
        file.close();
    }

    void browseFlights() {
        for (const auto& flight : flights) {
            cout << "Flight Number: " << flight.flightNumber << " - "
                << "From: " << flight.startPoint << " To: " << flight.endPoint
                << " Departure: " << flight.departureTime << "\n";
        }
    }

    void searchByFlightNumber(int flightNumber) {
        for (const auto& flight : flights) {
            if (flight.flightNumber == flightNumber) {
                cout << "Flight found: " << flightNumber << " - "
                    << "From: " << flight.startPoint << " To: " << flight.endPoint
                    << " Departure: " << flight.departureTime << "\n";
                return;
            }
        }
        cout << "Flight not found.\n";
    }

    void searchByEndPoint(const string& endPoint) {
        for (const auto& flight : flights) {
            if (flight.endPoint == endPoint) {
                cout << "Flight to " << endPoint << ": " << flight.flightNumber << " - "
                    << "Departure: " << flight.departureTime << "\n";
            }
        }
    }

    void bookTicket(int flightNumber) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff && flight.bookedPassengers < flight.maxCapacity) {
                flight.bookedPassengers++;
                cout << "Ticket booked for flight " << flightNumber << "\n";
                return;
            }
        }
        cout << "Cannot book ticket for flight " << flightNumber << "\n";
    }

    void cancelTicket(int flightNumber) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber && !flight.hasTakenOff && flight.bookedPassengers > 0) {
                flight.bookedPassengers--;
                cout << "Ticket cancelled for flight " << flightNumber << "\n";
                return;
            }
        }
        cout << "Cannot cancel ticket for flight " << flightNumber << "\n";
    }
};

// 用户界面函数
void userInterface(FlightManager& manager) {
    int choice;
    do {
        cout << "\n*** Flight Management System ***\n";
        cout << "1. Add Flight\n";
        cout << "2. Save Flights\n";
        cout << "3. Load Flights\n";
        cout << "4. Browse Flights\n";
        cout << "5. Search by Flight Number\n";
        cout << "6. Search by End Point\n";
        cout << "7. Book Ticket\n";
        cout << "8. Cancel Ticket\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int flightNumber;
        string endPoint;
        switch (choice) {
        case 1: {
            // 读取并添加航班信息
            Flight flight;
            cout << "Enter flight number, departure time, start point, end point, "
                << "flight duration, max capacity: ";
            cin >> flight.flightNumber >> flight.departureTime >> flight.startPoint
                >> flight.endPoint >> flight.flightDuration >> flight.maxCapacity;
            flight.bookedPassengers = 0;
            flight.hasTakenOff = false;
            manager.addFlight(flight);
            break;
        }
        case 2:
            manager.saveFlightsToFile("flights.dat");
            break;
        case 3:
            manager.loadFlightsFromFile("flights.dat");
            break;
        case 4:
            manager.browseFlights();
            break;
        case 5:
            cout << "Enter Flight Number to search: ";
            cin >> flightNumber;
            manager.searchByFlightNumber(flightNumber);
            break;
        case 6:
            cout << "Enter End Point to search: ";
            cin >> endPoint;
            manager.searchByEndPoint(endPoint);
            break;
        case 7:
            cout << "Enter Flight Number to book ticket: ";
            cin >> flightNumber;
            manager.bookTicket(flightNumber);
            break;
        case 8:
            cout << "Enter Flight Number to cancel ticket: ";
            cin >> flightNumber;
            manager.cancelTicket(flightNumber);
            break;
        }
    } while (choice != 0);
}

int main() {
    FlightManager manager;
    userInterface(manager);
    return 0;
}
