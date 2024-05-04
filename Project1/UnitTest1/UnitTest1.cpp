#include "pch.h"
#include "CppUnitTest.h"
#include "../Project1/Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(FileOperationsTests)
    {
    public:
        
      
        TEST_METHOD(ListSubscribersByPayment)
        {
            std::vector<Subscriber> subscribers = {
                {"Doe", "123 Main St."},
                {"Smith", "456 Elm St."}
            };
            std::vector<Phone> phones = {
                {"555-1234", "hv", 15.00},
                {"555-5678", "hv", 25.00}
            };
            double maxAmount = 20.00;

            std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

            listSubscribersByPayment(subscribers, phones, maxAmount);

            std::string output = buffer.str();
            std::string expectedOutput = "Subscribers with a charge per minute below $20:\n";
            expectedOutput += "Last name: Doe, Address: 123 Main St., Phone: 555-1234\n";

            Assert::AreEqual(expectedOutput, output);

            std::cout.rdbuf(old);
        }

        TEST_METHOD(FindPhoneNumberByAddress)
        {
            std::vector<Subscriber> subscribers = {
                {"Doe", "123 Main St."},
                {"Smith", "456 Elm St."}
            };
            std::vector<Phone> phones = {
                {"555-1234", "hv", 15.00},
                {"555-5678", "hv", 25.00}
            };
            std::string address = "123 Main St.";

            std::string phoneNumber = findPhoneNumberByAddress(subscribers, phones, address);

            Assert::AreEqual(std::string("555-1234"), phoneNumber);
        }

    };
}
