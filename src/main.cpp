#include <iostream>
using namespace std;

#include "utils/logger.h"
#include "network/netcore.h"
#include "fstream"
#include "pb/main.pb.h"

// This function fills in a Person message based on user input.
// void PromptForAddress(tutorial::Person* person) {
//   cout << "Enter person ID number: ";
//   int id;
//   cin >> id;
//   person->set_id(id);
//   cin.ignore(256, '\n');

//   cout << "Enter name: ";
//   getline(cin, *person->mutable_name());

//   cout << "Enter email address (blank for none): ";
//   string email;
//   getline(cin, email);
//   if (!email.empty()) {
//     person->set_email(email);
//   }

//   while (true) {
//     cout << "Enter a phone number (or leave blank to finish): ";
//     string number;
//     getline(cin, number);
//     if (number.empty()) {
//       break;
//     }

//     tutorial::Person::PhoneNumber* phone_number = person->add_phones();
//     phone_number->set_number(number);

//     cout << "Is this a mobile, home, or work phone? ";
//     string type;
//     getline(cin, type);
//     if (type == "mobile") {
//       phone_number->set_type(tutorial::Person::MOBILE);
//     } else if (type == "home") {
//       phone_number->set_type(tutorial::Person::HOME);
//     } else if (type == "work") {
//       phone_number->set_type(tutorial::Person::WORK);
//     } else {
//       cout << "Unknown phone type.  Using default." << endl;
//     }
//   }
// }

// // Main function:  Reads the entire address book from a file,
// //   adds one person based on user input, then writes it back out to the same
// //   file.
// int main(int argc, char* argv[]) {
//   // Verify that the version of the library that we linked against is
//   // compatible with the version of the headers we compiled against.
//   GOOGLE_PROTOBUF_VERIFY_VERSION;
//   string fname = "address_book";

//   tutorial::AddressBook address_book;

//   {
//     // Read the existing address book.
//     fstream input(fname, ios_base::in | ios_base::binary);
//     if (!input) {
//       cout << fname << ": File not found.  Creating a new file." << endl;
//     } else if (!address_book.ParseFromIstream(&input)) {
//       cerr << "Failed to parse address book." << endl;
//       return -1;
//     }
//   }

//   // Add an address.
//   PromptForAddress(address_book.add_people());

//   {
//     // Write the new address book back to disk.
//     fstream output(fname, ios_base::out | ios_base::trunc | ios_base::binary);
//     if (!address_book.SerializeToOstream(&output)) {
//       cerr << "Failed to write address book." << endl;
//       return -1;
//     }
//   }

//   // Optional:  Delete all global objects allocated by libprotobuf.
//   google::protobuf::ShutdownProtobufLibrary();

//   return 0;
// }

int main(){
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  init_fnet_logger("../logs/ty");
  FLOG(warning) << "system start";
  fnet::OutMsgBuffer buf;
  buf.set_msg_type(99);
  buf.set_msg_extra(0x61626364);
  buf.msg_content = std::move(std::string("hello world"));
  std::cout << std::string(buf.get_head(), buf.get_head_size()) << std::endl;
  fnet::NetCore nc;
  nc.start(7777);
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
