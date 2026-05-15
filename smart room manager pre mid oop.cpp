#include <iostream>
#include <fstream>
using namespace std;

class Device
{
private:
    string name;
    bool isOn;
    int brightness;

public:
    // Default Constructor
    Device()
    {
        name = "Unknown";
        isOn = false;
        brightness = 50;
    }

    // Overloaded Constructor
    Device(string n, bool status, int b)
    {
        name = n;
        isOn = status;
        setBrightness(b);
    }

    // Destructor
    ~Device()
    {
        cout << "Device " << name << " is being removed." << endl;
    }

    // Setters and Getters
    //For name
    void setName(string n)
    {
        name = n;
    }

    string getName() const
    {
        return name;
    }
    //For brightness
    void setBrightness(int b)
    {
        if (b >= 0 && b <= 100)
            brightness = b;
        else
            brightness = 50;
    }

    int getBrightness() const
    {
        return brightness;
    }
    //For turning on & off
    void turnOn()
    {
        isOn = true;
    }

    void turnOff()
    {
        isOn = false;
    }
    //For status
    bool getStatus() const
    {
        return isOn;
    }
    //For info
    void showInfo() const
    {
        cout << "Name: " << name
            << ", Status: " << (isOn ? "ON" : "OFF")
            << ", Brightness: " << brightness << endl;
    }

    // Save device to file
    void saveToFile(ofstream& file) //& helps to save it as address
    {
        file << name << " " << isOn << " " << brightness << endl; //writing into file
    }

   
};

class Room
{
private:
    string roomName;
    Device devices[5]; //array of objects
    int count;

public:
    //constructor
    Room(string name)
    {
        roomName = name;
        count = 0;
    }
    //destructor
    ~Room()
    {
        cout << "Room " << roomName << " manager closed." << endl;
    }
    //to add devices
    void addDevice(Device d)
    {
        if (count < 5)
        {
            devices[count] = d; //stores device in array
            count++;
            cout << "Device added successfully.\n";
        }
        else
            cout << "Room is full!\n";
    }
    //to show devices
    void showAllDevices() const
    {
        cout << "Total devices = " << count << endl;   

        if (count == 0)
        {
            cout << "No devices in room.\n";ss
            return;
        }

        for (int i = 0; i < count; i++)
            devices[i].showInfo();  //display each device
    }
    //to save data in file
    void saveAll()
    {
        //to write data in file
        ofstream file("devices.txt");

        if (!file)
        {
            cout << "File error while saving\n";
            return;
        }

        for (int i = 0; i < count; i++)
            devices[i].saveToFile(file);

        file.close();

        cout << "Saved " << count << " devices to file\n";
    }
    //to load file
    void loadFromFile()
    {
        //to read data from file
        ifstream file("devices.txt");

        if (!file)
        {
            cout << "No previous file found.\n";
            return;
        }

        string name;
        bool status;
        int bright;
        
        while (file >> name >> status >> bright) //reading from file
        {
            Device d(name, status, bright);

            if (count < 5)
            {
                devices[count] = d;
                count++;
            }
        }

        file.close();

        cout << "Loaded " << count << " devices from file\n";
    }

    int findDevice(string name)  //to find devices
    {
        for (int i = 0; i < count; i++)
            if (devices[i].getName() == name)
                return i;

        return -1;
    }

    void toggleDevice(string name)   //to toggle (on & off)
    {
        int index = findDevice(name);

        if (index == -1)
        {
            cout << "Device not found\n";
            return;
        }

        if (devices[index].getStatus())
            devices[index].turnOff();
        else
            devices[index].turnOn();
    }

    void setDeviceBrightness(string name, int b) //for brightness
    {
        int index = findDevice(name);

        if (index == -1)
        {
            cout << "Device not found\n";
            return;
        }

        devices[index].setBrightness(b);
    }
};

int main()
{
    Room room("LivingRoom");
    room.loadFromFile();   // Load old devices firsts

    int choice;

    do
    {
        cout << " Smart Room Manager" << endl;
        cout << "1. Add a Device" << endl;
        cout << "2. Show All Device" << endl;
        cout << "3. Turn Device ON/OFF " << endl;
        cout << "4. Change Brightness " << endl;
        cout << "5. Exit " << endl;
        cout << "Enter choice: " << endl;
        cin >> choice;

        if (choice == 1)
        {
            string name;
            int brightness;

            cout << "Enter device name: ";
            cin >> name;

            cout << "Enter brightness (0-100): ";
            cin >> brightness;

            Device d(name, false, brightness);
            room.addDevice(d);
        }

        else if (choice == 2)
        {
            room.showAllDevices();
        }

        else if (choice == 3)
        {
            string name;
            cout << "Enter device name: ";
            cin >> name;
            room.toggleDevice(name);
        }

        else if (choice == 4)
        {
            string name;
            int brightness;
            cout << "Enter device name: ";
            cin >> name;
            cout << "Enter new brightness: ";
            cin >> brightness;
            room.setDeviceBrightness(name, brightness);
        }

    } while (choice != 5);

    room.saveAll();  //to save all devices

    cout << " Exiting program "<<endl;

    return 0;
}