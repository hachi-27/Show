#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <regex>
#include <sstream>

#include "ccapi_cpp/ccapi_session.h"
namespace ccapi {
class MyEventHandler : public EventHandler {
 public:
  explicit MyEventHandler(const std::string& filename, const std::map<std::string, std::string>& symbolByCorrelationIdMap)
      : f(filename), symbolByCorrelationIdMap(symbolByCorrelationIdMap) {
    f << "Timestamp,Symbol,Bid_Price1, Bid_Price2, Bid_Price3, Bid_Price4, Bid_Price5, Bid_Price6,Bid_Price7, Bid_Price8, Bid_Price9, Bid_Price10, Bid_Size1, "
         "Bide_Size2, Bid_Size3,Bid_Size4, Bid_Size5, Bid_Size6, Bid_Size7, Bid_Size8, Bid_Size9, Bid_Size10, Ask_Price1, Ask_Price2, Ask_Price3, Ask_Price4, "
         "Ask_Price5, Ask_Price6, Ask_Price7, Ask_Price8, Ask_Price9, Ask_Price10, Ask_Size1, Ask_Size2, Ask_Size3, Ask_Size4, Ask_Size5, Ask_Size6, "
         "Ask_Size7, Ask_Size8, Ask_Size9, Ask_Size10\n";
    f.flush();
  }
    std::vector<std::string> OptionStrikes;
  auto DayPlusDate() {
    using namespace boost::gregorian;
    std::string check = "Fri";
    std::vector<std::string> output;
    auto now = boost::posix_time::second_clock::local_time();
    for (int i = 1; i < 18; i++) {
      auto today = now.date();
      boost::gregorian::date d(today.year(), today.month(), today.day());
      d += days(i);
      if (check == boost::lexical_cast<std::string>(d.day_of_week())) {
        // create your formatting
        boost::gregorian::date_facet* df = new boost::gregorian::date_facet("%d%b%y");

        // set your formatting
        std::ostringstream is;
        is.imbue(std::locale(is.getloc(), df));
        is << d;
        output.push_back(is.str());
        for (std::string& s : output) {
          std::transform(s.begin(), s.end(), s.begin(), [](char c) { return std::toupper(c); });  // for lowercase change "toupper" -> "tolower"
        }
      }
    }
    return output;
  }
  std::vector<std::string> InstrumentVec(std::string tmp) {
    std::vector<std::string> output;
    std::vector<std::string> WeeklyDate;
    const char* s = buffer.c_str();
    while (*s) {
      while (*s && *s != ':') {
        key.push_back(*s);
        ++s;
      }
      if (!*s) {
        break;
      }
      s += 1;
      // parse the value
      while (*s && *s != ',') {
        value.push_back(*s);
        ++s;
      }
      ret[key].push_back(value);
      key.clear();
      value.clear();
      ++s;
    }
    WeeklyDate = DayPlusDate();
    for (auto& x : ret) {
      if (x.first == "\"instrument_name\"") {
        for (auto& c : x.second) {
          if (((c.find(WeeklyDate[0]) != std::string::npos)) || (c.find(WeeklyDate[1]) != std::string::npos)) {
            output.push_back(c);
          }
        }
      }
    }

    return output;
  }
  bool processEvent(const Event& event, Session* session) override {
    if (event.getType() == Event::Type::SUBSCRIPTION_DATA) {
      for (const auto& message : event.getMessageList()) {
        auto messageTime = message.getTime();
        auto messageTimePair = UtilTime::divide(messageTime);
        auto aCorrelationId = message.getCorrelationIdList()[0];
        buffer.clear();
        buffer_1.clear();
        auffer.clear();
        auffer_1.clear();
        std::vector<ccapi::Element> elem = message.getElementList();
        buffer += std::to_string(messageTimePair.first);
        buffer += ",";
        buffer += symbolByCorrelationIdMap.at(aCorrelationId);
        buffer += ",";
        // auto mid = elem.begin() + elem.size()/2;
        // for (const auto& element : message.getElementList()) {
        for (auto it = std::begin(elem), mid = std::begin(elem) + elem.size() / 2; it != std::end(elem) && mid != std::end(elem); it++, mid++) {
          const std::map<std::string, std::string>& elementNameValueMap = (*it).getNameValueMap();
          const std::map<std::string, std::string>& elementNameValueMap1 = (*mid).getNameValueMap();
          CCAPI_LOGGER_TRACE("elementNameValueMap = " + toString(elementNameValueMap));
          if (elementNameValueMap.find(CCAPI_BEST_BID_N_PRICE) != elementNameValueMap.end()) {
            buffer += elementNameValueMap.at(CCAPI_BEST_BID_N_PRICE);
            buffer += ",";
            buffer_1 += elementNameValueMap.at(CCAPI_BEST_BID_N_SIZE);
            buffer_1 += ",";
          }

          if (elementNameValueMap1.find(CCAPI_BEST_ASK_N_PRICE) != elementNameValueMap1.end()) {
            auffer += elementNameValueMap1.at(CCAPI_BEST_ASK_N_PRICE);
            auffer += ",";
            auffer_1 += elementNameValueMap1.at(CCAPI_BEST_ASK_N_SIZE);
            auffer_1 += ",";
          }
        }
        buffer = buffer + buffer_1 + auffer + auffer_1;
        buffer += "\n";
        f << buffer;
        f.flush();
      }
    } else if (event.getType() == Event::Type::RESPONSE) {
      for (const auto& message : event.getMessageList()) {
        for (auto& element : message.getElementList()) {
          const std::map<std::string, std::string>& elementNameValueMap = element.getNameValueMap();
          if (elementNameValueMap.find("HTTP_BODY") != elementNameValueMap.end()) {
            tmp += elementNameValueMap.at("HTTP_BODY");
          }
        }
      }
      OptionStrikes = InstrumentVec(tmp);
     
          
    } 
	else {
      std::cout << "Invalid Event" << std::endl;
    }
    return true;
  }

 private:
  std::ofstream f;
  std::string buffer, buffer_1, auffer, auffer_1;
  std::map<std::string, std::string> symbolByCorrelationIdMap;
  std::string tmp;
  std::map<std::string, std::vector<std::string>> ret;
  std::string key;
  std::string value;
  const char* s;
 // std::vector<std::string> OptionStrikes;
};
} /* namespace ccapi */
using ::ccapi::MyEventHandler;
using ::ccapi::Request;
using ::ccapi::Session;
using ::ccapi::SessionConfigs;
using ::ccapi::SessionOptions;
using ::ccapi::Subscription;
using ::ccapi::toString;
int main(int argc, char** argv) {
  std::map<std::string, std::string> symbolByCorrelationIdMap;
  int correlationId = 0;
  std::vector<Subscription> subscriptionList;
  std::string subscriptionOptions =
      std::string(CCAPI_MARKET_DEPTH_MAX) + "=10&" + CCAPI_CONFLATE_INTERVAL_MILLISECONDS + "=3000&" + CCAPI_CONFLATE_GRACE_PERIOD_MILLISECONDS + "=0";
  SessionOptions sessionOptions;
  SessionConfigs sessionConfigs;
  MyEventHandler eventHandler("hello.csv", symbolByCorrelationIdMap);
  Session session(sessionOptions, sessionConfigs, &eventHandler);
   ccapi::Request request(ccapi::Request::Operation::GENERIC_PUBLIC_REQUEST, "deribit", "", "Option Data");
  request.appendParam({
      {"HTTP_METHOD", "GET"},
      {"HTTP_PATH", "/api/v2/public/get_book_summary_by_currency"},
      {"HTTP_QUERY_STRING", "currency=BTC&kind=option "},
  });
  session.sendRequest(request);
  std::this_thread::sleep_for(std::chrono::seconds(1));
 
  //OptionStrikes is defined public in MyEventHandler class and should keep it's values saved as per it's definition
  std::vector<std::string> OptionInst = eventHandler.OptionStrikes;

  for (auto& elem : OptionInst) {
    Subscription subscription("deribit", elem, "MARKET_DEPTH", subscriptionOptions, std::to_string(correlationId));
    symbolByCorrelationIdMap[std::to_string(correlationId)] = elem;
    correlationId++;
    subscriptionList.push_back(subscription);
  }
 // Session session(sessionOptions, sessionConfigs, &eventHandler);
  session.subscribe(subscriptionList);
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(INT_MAX));
  }
  session.stop();
  std::cout << "Bye" << std::endl;
  return EXIT_SUCCESS;
} 
