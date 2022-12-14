// SPDX-License-Identifier: MIT
/**
   Copyright (c) 2015 - 2021 Beckhoff Automation GmbH & Co. KG
 */

#include "AdsLib/AdsLib.h"
#include "AdsLib/AdsNotificationOOI.h"
#include "AdsLib/AdsVariable.h"
#include "ros/ros.h"
#include "ros/console.h"

#include <array>
#include <iostream>
#include <iomanip>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void NotifyCallback(const AmsAddr* pAddr, const AdsNotificationHeader* pNotification, uint32_t hUser)
{
    const uint8_t* data = reinterpret_cast<const uint8_t*>(pNotification + 1);
    std::cout << std::setfill('0') <<
        "NetId: " << pAddr->netId <<
        " hUser 0x" << std::hex << hUser <<
        " sample time: " << std::dec << pNotification->nTimeStamp <<
        " sample size: " << std::dec << pNotification->cbSampleSize <<
        " value:";
    for (size_t i = 0; i < pNotification->cbSampleSize; ++i) {
        std::cout << " 0x" << std::hex << (int)data[i];
    }
    std::cout << '\n';
}

static void notificationExample(std::ostream& out, const AdsDevice& route)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERCYCLE,
        0,
        {4000000}
    };
    AdsNotification notification { route, 0x4020, 4, attrib, &NotifyCallback, 0xDEADBEEF };

    out << "Hit ENTER to stop notifications\n";
    std::cin.ignore();
}

static void notificationByNameExample(std::ostream& out, const AdsDevice& route)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERCYCLE,
        0,
        {4000000}
    };

    out << __FUNCTION__ << "():\n";
    //AdsNotification notification { route, "MAIN.byByte[4]", attrib, &NotifyCallback, 0xBEEFDEAD };
    AdsNotification notification { route, "MAIN.i", attrib, &NotifyCallback, 0xBEEFDEAD };

    out << "Hit ENTER to stop by name notifications\n";
    std::cin.ignore();
}

static void readExample(std::ostream& out, const AdsDevice& route)
{
    AdsVariable<uint8_t> readVar {route, 0x4020, 0};

    out << __FUNCTION__ << "():\n";
    for (size_t i = 0; i < 8; ++i) {
        out << "ADS read " << std::hex << (uint32_t)readVar << '\n';
    }
}

static void readByNameExample(std::ostream& out, const AdsDevice& route)
{
    //AdsVariable<uint8_t> readVar {route, "MAIN.byByte[4]"};
    AdsVariable<uint8_t> readVar {route, "MAIN.i"};

    out << __FUNCTION__ << "():\n";
    for (size_t i = 0; i < 8; ++i) {
        out << "ADS read " << std::hex << (uint32_t)readVar << '\n';
    }
}

static void readWriteExample(std::ostream& out, const AdsDevice& route)
{
    //AdsVariable<uint8_t> simpleVar {route, "MAIN.byByte[0]"};
    //AdsVariable<uint8_t> validation {route, "MAIN.byByte[0]"};
    AdsVariable<uint8_t> testVar {route, "MAIN.writeVar"};

    out << __FUNCTION__ << "():\n";
    testVar = 42;
    out << "Wrote " << 42 << " to MAIN.byByte and read " << (uint32_t)testVar << " back\n";
    //simpleVar = 0xA5;
    //out << "Wrote " << 0xA5 << " to MAIN.byByte and read " << (uint32_t)validation << " back\n";
    //simpleVar = 0x5A;
    //out << "Wrote " << (uint32_t)simpleVar << " to MAIN.byByte and read " << (uint32_t)validation << " back\n";
}

static void testADSExample(std::ostream& out, const AdsDevice& route)
{
    AdsVariable<uint8_t> testVar {route, "MAIN.writeVar"};
    AdsVariable<uint8_t> testreadVar {route, "MAIN.readVar"};
    for (int i = 0; i < 5; i++) {
        //AdsVariable<uint8_t> testVar {route, "MAIN.writeVar"};
        //if ((uint32_t)testVar == 0){
            testVar = i;
        //}

        out << "Wrote " << (uint32_t)testVar  << " to MAIN.writeVar\n";
        out << "Read " << (uint32_t)testreadVar  << " from MAIN.readVar\n";
    }
}

static void readWriteArrayExample(std::ostream& out, const AdsDevice& route)
{
    static const std::array<uint8_t, 10> arrayToWrite = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    AdsVariable<std::array<uint8_t, 10> > arrayVar {route, "MAIN.byByte"};

    //out << "Read back array with first value " << (uint32_t)arrayVar[0] << " and last value " <<
    //    (uint32_t)arrayVar[3] << "\n";

    arrayVar = arrayToWrite;

    std::array<uint8_t, 10> readArray = arrayVar;
    //out << "Wrote array with first value " << (uint32_t)arrayToWrite[0] << " and last value " <<
    //    (uint32_t)arrayToWrite[3] << "\n";
    out << "Read back array with first value " << (uint32_t)readArray[0] << " and last value " <<
        (uint32_t)readArray[3] << "\n";
}

static void readStateExample(std::ostream& out, const AdsDevice& route)
{
    const auto state = route.GetState();

    out << "ADS state: " << std::dec << (uint16_t)state.ads << " devState: " << std::dec << (uint16_t)state.device <<
        '\n';
}

static void runExample(std::ostream& out)
{
    static const AmsNetId remoteNetId { 10, 199, 109, 140, 1, 1 };
    static const char remoteIpV4[] = "ads-server";

    // uncomment and adjust if automatic AmsNetId deduction is not working as expected
    //bhf::ads::SetLocalAddress({192, 168, 0, 1, 1, 1});

    AdsDevice route {remoteIpV4, remoteNetId, AMSPORT_R0_PLC_TC3};
    notificationExample(out, route);
    notificationByNameExample(out, route);
    readExample(out, route);
    readByNameExample(out, route);
    readWriteExample(out, route);
    readWriteArrayExample(out, route);
    readStateExample(out, route);
    testADSExample(out, route);
}

int main(int argc, char **argv)
{
    

    try {
        ros::init(argc, argv, "example");
        ros::NodeHandle n;   
        runExample(std::cout);

        ros::spin();
    } catch (const AdsException& ex) {
        std::cout << "Error: " << ex.errorCode << "\n";
        std::cout << "AdsException message: " << ex.what() << "\n";
    } catch (const std::runtime_error& ex) {
        std::cout << ex.what() << '\n';
    }
    std::cout << "Hit ENTER to continue\n";
    std::cin.ignore();
}
#pragma GCC diagnostic pop
